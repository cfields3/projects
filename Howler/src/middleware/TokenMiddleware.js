const crypto = require('crypto'); // For Hmac cryptographic functions
const base64url = require('base64url'); // For encoding in base64url
const dotenv = require('dotenv'); // For reading secret key
const exp = require('constants');

dotenv.config(); // Set up environment variables

// Load secret key from list of environment variables in .env
// (excluded from version control to maintain secrecy)
const API_SECRET = process.env.API_SECRET;

const TOKEN_COOKIE_NAME = "currentUser";

// Note: the following code is a modification of in-class lecture
// code in order to use similar middleware functionality but without
// using the jsonwebtoken package

exports.TokenMiddleware = (req, res, next) => {
  // We will look for the token in two places:
  // 1. A cookie in case of a browser
  // 2. The Authorization header in case of a different client
  let token = null;
  if(!req.cookies[TOKEN_COOKIE_NAME]) {
    // No cookie, so let's check Authorization header
    const authHeader = req.get('Authorization');
    if(authHeader && authHeader.startsWith("Bearer ")) {
      // Format should be "Bearer token" but we only need the token
      token = authHeader.split(" ")[1];
    }
  }
  else { // We do have a cookie with a token
    token = req.cookies[TOKEN_COOKIE_NAME]; //Get session Id from cookie
  }

  if(!token) { // If we don't have a token
    res.status(401).json({error: 'Not authenticated'});
    return;
  }

  // If we've made it this far, we have a token. We need to validate it

  // Extract important information from the token
  let headerBase64URL = token.split('.')[0];
  let payloadBase64URL = token.split('.')[1];
  let payload = JSON.parse(base64url.decode(payloadBase64URL));
  let signature = token.split('.')[2];
  let message = headerBase64URL + "." + payloadBase64URL;
  
  // Calculate what the signature should be given the header & payload
  let expectedSignature = crypto.createHmac('sha256', API_SECRET).update(message).digest('base64url');

  // Token is invalid if signature is invalid or if token is expired
  if (signature != expectedSignature || Math.floor(Date.now() / 1000) >= payload.exp) {
    res.status(401).json({error: 'Not authenticated'});
    return;
  }

  // Token is valid; update the request
  req.user = payload.user;

  next(); // Make sure we call the next middleware
}


exports.generateToken = (req, res, user) => {
  let header = {
    alg: 'HS256',
    typ: 'JWT'
  };

  let payload = {
    user: user,
    // Use the exp registered claim to expire token in 10 minutes
    exp: Math.floor(Date.now() / 1000) + (10 * 60),
    iat: Math.floor(Date.now() / 1000)
  };

  let headerBase64URL = base64url(JSON.stringify(header));
  let payloadBase64URL = base64url(JSON.stringify(payload));

  let message = headerBase64URL + "." + payloadBase64URL;

  let signature = crypto.createHmac('sha256', API_SECRET).update(message).digest('base64url');

  const token = headerBase64URL + "." + payloadBase64URL + "." + signature;

  //send token in cookie to client
  res.cookie(TOKEN_COOKIE_NAME, token, {
    httpOnly: true,
    secure: true,
    maxAge: 10 * 60 * 1000 // Session expires in 10 minutes (this is the same as the token expiration)
  });
};


exports.removeToken = (req, res) => {
  // send session ID in cookie to client
  res.cookie(TOKEN_COOKIE_NAME, "", {
    httpOnly: true,
    secure: true,
    maxAge: -360000 // A date in the past
  });
}

