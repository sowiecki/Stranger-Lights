const http = require('http');
const finalhandler = require('finalhandler');
const Router = require('router');
const Twitter = require('twitter');

const ENV = require('./env');
const { PORT, DEFAULT_MESSAGE } = require('./constants');
const { convertTweetToLetterIndexes } = require('./utils');

const router = Router();
const client = new Twitter(ENV.twitter);

const getLatestTweet = (res, req) => {
  const screenName = req.params.screen_name || ENV.screen_name;

  client.get('search/tweets', { q: screenName }, (error, tweets, response) => {
    const latestTweet =
      tweets.statuses && tweets.statuses.length > 0
        ? tweets.statuses[0]
        : { text: DEFAULT_MESSAGE };

    res.setHeader('Content-Type', 'application/json');
    res.end(convertTweetToLetterIndexes(latestTweet, screenName));
  });
};

const run = () => {
  router.get('/:screen_name?', (req, res) => {
    getLatestTweet(res, req);
  });

  const server = http.createServer((req, res) => {
    router(req, res, finalhandler(req, res));
  });

  server.listen(PORT);
};

run();
