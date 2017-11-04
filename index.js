const http = require('http');
const moment = require('moment');
const finalhandler = require('finalhandler');
const Router = require('router');
const Twitter = require('twitter');

const ENV = require('./env');
const { PORT, DEFAULT_MESSAGE, TWEET_EXPIRED } = require('./constants');
const {
  convertTweetToLetterIndexes,
  getTime,
  getTimeSinceLastTweet,
  getScreenName
} = require('./utils');

const router = Router();
const client = new Twitter(ENV.twitter);

const state = {
  timeSinceLastTweet: getTime().subtract(TWEET_EXPIRED, 'seconds'),
  storedTweet: { text: 'RUN' }
};

const getLatestTweet = (res, req) => {
  const screenName = getScreenName(res, req);

  client.get('search/tweets', { q: screenName }, (error, tweets, response) => {
    const latestTweet =
      tweets.statuses && tweets.statuses.length > 0
        ? tweets.statuses[0]
        : { text: DEFAULT_MESSAGE };

    res.setHeader('Content-Type', 'application/json');
    res.end(convertTweetToLetterIndexes(latestTweet, screenName));

    state.storedTweet = latestTweet;
  });
};

const run = () => {
  router.get('/:screen_name?', (req, res) => {
    const storedTweetIsExpired = getTime().isAfter(
      moment(state.timeSinceLastTweet).add(TWEET_EXPIRED, 'seconds')
    );

    if (storedTweetIsExpired) {
      state.timeSinceLastTweet = getTime();

      getLatestTweet(res, req);
    } else {
      const screenName = getScreenName(res, req);
      res.setHeader('Content-Type', 'application/json');
      res.end(convertTweetToLetterIndexes(state.storedTweet, screenName));
    }
  });

  const server = http.createServer((req, res) => {
    router(req, res, finalhandler(req, res));
  });

  server.listen(PORT);
};

run();
