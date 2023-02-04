const jwt = require('jsonwebtoken');

const TOKEN_COOKIE_NAME = "PackProducerToken";
const API_SECRET = process.env.API_SECRET_KEY;

exports.TokenMiddleware = (req, res, next) => {
  let token = null;
  if (!req.cookies[TOKEN_COOKIE_NAME]) {
    const authHeader = req.get('Authorization');
    if (authHeader && authHeader.startsWith("Bearer "))
      token = authHeader.split(" ")[1];
  }
  else {
    token = req.cookies[TOKEN_COOKIE_NAME];
  }

  if (!token) {
    res.status(401).json({error: 'Not authenticated'});
    return;
  }

  try {
    const decoded = jwt.verify(token, API_SECRET);
    req.user = decoded.user;
    next();
  }
  catch(err) {
    res.status(401).json({error: 'Not authenticated'});
    return;
  }
}

exports.generateToken = (req, res, user) => {
  let data = {
    user: user,
    exp: Math.floor(Date.now() / 1000) + (60 * 30) // Set token to expire in 30 minutes
  }

  const token = jwt.sign(data, API_SECRET);

  res.cookie(TOKEN_COOKIE_NAME, token, {
    httpOnly: true,
    secure: true,
    maxAge: 30 * 60 * 1000 // Token also lasts 30 minutes
  });
};

exports.removeToken = (req, res) => {
  res.cookie(TOKEN_COOKIE_NAME, "", {
    httpOnly: true,
    secure: true,
    maxAge: -360000
  });
};
