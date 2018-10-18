const http = require('http');
const moment = require('moment');
const finalhandler = require('finalhandler');
const Router = require('router');
const Twitter = require('twitter');
const { isEmpty } = require('lodash');

const ENV = require('./env');
const {
  PORT,
  DEFAULT_MESSAGE,
  TWEET_EXPIRED,
  TWITTER_DATE_FORMAT
} = require('./constants');
const {
  convertTweetToLetterIndexes,
  getTimeSinceLastTweet,
  getScreenName
} = require('./utils');

const router = Router();
const client = new Twitter(ENV.twitter);

const state = {
  timeSinceLastTweet: moment().subtract(TWEET_EXPIRED, 'seconds'),
  storedTweets: { default: { text: 'RUN' } }
};

const filterStatuses = statuses =>
  statuses
    .filter(
      status => status.retweet_count === 0 && isEmpty(status.retweeted_status)
    )
    .sort((a, b) => {
      return moment(a.created_at, TWITTER_DATE_FORMAT).isBefore(
        moment(b.created_at, TWITTER_DATE_FORMAT)
      );
    });

const getLatestTweet = (res, req) => {
  const screenName = getScreenName(res, req);

  client.get(
    'search/tweets',
    { q: screenName, result_type: 'recent' },
    (error, tweets, response) => {
      const filteredStatuses = filterStatuses(tweets.statuses);
      const latestTweet =
        filteredStatuses && filteredStatuses.length > 0
          ? filteredStatuses[0]
          : { text: DEFAULT_MESSAGE };

      res.setHeader('Content-Type', 'application/json');
      res.end(convertTweetToLetterIndexes(latestTweet, screenName));

      state.storedTweets[screenName] = latestTweet;
    }
  );
};

const run = () => {
  router.get('/:screen_name?', (req, res) => {
    const screenName = getScreenName(res, req);
    const cacheCounterExpired = moment().isAfter(
      moment(state.timeSinceLastTweet).add(TWEET_EXPIRED, 'seconds')
    );

    if (cacheCounterExpired || !state.storedTweets[screenName]) {
      state.timeSinceLastTweet = moment();

      getLatestTweet(res, req);
    } else {
      res.setHeader('Content-Type', 'application/json');
      res.end(
        convertTweetToLetterIndexes(state.storedTweets[screenName], screenName)
      );
    }
  });

  const server = http.createServer((req, res) => {
    router(req, res, finalhandler(req, res));
  });

  server.listen(PORT);
};

run();
