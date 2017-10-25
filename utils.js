const ENV = require('./env');

module.exports.convertTweetToLetterIndexes = (
  { text },
  screenName = ENV.screen_name
) => {
  const indexes = text
    .toLowerCase()
    .replace(`@${screenName.toLowerCase()}`, '')
    .split('')
    .map(letter => letter.charCodeAt(0) - 97)
    .filter(index => index >= 0 && index <= 26);

  return JSON.stringify(indexes);
};
