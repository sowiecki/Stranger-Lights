const getEnv = () => {
  try {
    return require('./env.json');
  } catch (e) {
    console.log(`${e.message}, falling back to process.env`);

    return {
      twitter: JSON.parse(process.env.twitter),
      screen_name: ''
    };
  }
};

module.exports = getEnv();
