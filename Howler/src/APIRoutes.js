const express = require('express');
const apiRouter = express.Router();
const db = require('./data/authentication');
const crypto = require('crypto');

let follows = require('./data/follows.json');
let howls = require('./data/howls.json');
let users = require('./data/users.json');

const cookieParser = require('cookie-parser');
apiRouter.use(cookieParser());

apiRouter.use(express.json());

const {TokenMiddleware, generateToken, removeToken} = require('./middleware/TokenMiddleware');

function sortByDate(a, b) {
  const dateA = new Date(a.datetime);
  const dateB = new Date(b.datetime);

  if (dateA > dateB)
    return -1;
  else if (dateB < dateA)
    return 1;
  else
    return 0;
};

// Get the currently authenticated user's object
apiRouter.get('/users/current', TokenMiddleware, (req, res) => {
  res.json(req.user);
});

// Create a new howl
apiRouter.post('/howls', TokenMiddleware, (req, res) => {
  let newHowl = req.body;
  howls.push(newHowl);
  res.json(newHowl);
});

// Get howls posted by a specific user
apiRouter.get('/users/:userId/howls', TokenMiddleware, (req, res) => {
  const userId = req.params.userId;
  const results = howls.filter(howl => howl.userId == userId);
  results.sort(sortByDate);
  res.json(results);
});

// Get howls posted by all users followed by the "authenticated" user
apiRouter.get('/users/current/follows/howls', TokenMiddleware, (req, res) => {
  let userFollows = follows[req.user.id].following;
  let followedHowls = [];
  userFollows.forEach(followerId => howls.filter(howl => howl.userId == followerId)
  .forEach(followedHowl => followedHowls.push(followedHowl)));
  followedHowls.sort(sortByDate);
  res.json(followedHowls);
});

// Get a specific user's object
apiRouter.get('/users/:userId', TokenMiddleware, (req, res) => {
  const userIdParam = req.params.userId;
  let user = users.find(user => user.id == userIdParam);
  if(user) {
    res.json(user);
  }
  else {
    res.status(404).json({error: 'User not found'});
  }
});

// Get list of users followed by a specific user
apiRouter.get('/users/:userId/follows', TokenMiddleware, (req, res) => {
  const userIdParam = req.params.userId;
  let user = users.find(user => user.id == userIdParam);
  if(!user)
    res.status(404).json({error: 'User not found'});
  let userFollows = follows[userIdParam].following;
  if(userFollows) {
    res.json(userFollows);
  }
  else {
    res.status(404).json({error: 'User\'s followers not found'});
  }
});

// Follow a user
apiRouter.get('/users/:userId/follow', TokenMiddleware, (req, res) => {
  let currentUser = req.user;
  const userIdParam = req.params.userId;
  let user = users.find(user => user.id == userIdParam);
  if(!user) {
    res.status(404).json({error: 'User not found'});
  } else {
    follows[currentUser.id].following.push(parseInt(userIdParam, 10));
    res.json(follows[currentUser.id].following);
  } 
  
});

// Unfollow a user
apiRouter.get('/users/:userId/unfollow', TokenMiddleware, (req, res) => {
  let currentUser = req.user;
  const userIdParam = req.params.userId;
  let user = users.find(user => user.id == userIdParam);
  if(!user) {
    res.status(404).json({error: 'User not found'});
  } else {
    let userFollows = follows[currentUser.id].following;
    let idIndex = userFollows.indexOf(parseInt(userIdParam, 10));
    if (idIndex == -1) {
      res.status(404).json({error: 'User with ID not currently followed'});
    } else {
      userFollows.splice(idIndex, 1);
      res.json(follows[currentUser.id].following);
    }
  }
});

// Get the number of howls in the system
apiRouter.get('/howls/count', TokenMiddleware, (req, res) => {
  res.json({howlID: howls.length});
});


/* USER AUTHENTICATION ROUTES */

// Authenticate a user
apiRouter.post('/users/login', (req, res) => {
  if(req.body.username && req.body.password) {
    db.getUserByCredentials(req.body.username, req.body.password).then(user => {
      let result = {
        user: user
      }

      generateToken(req, res, user);

      res.json(result);
    }).catch(err => {
      res.status(400).json({error: err});
    });
  }
  else {
    res.status(401).json({error: 'Not authenticated'});
  }
});

// Log out a user
apiRouter.post('/users/logout', (req, res) => {
  removeToken(req, res);

  res.json({success: true}); 
});

// Create a new user
apiRouter.post('/users', (req, res) => {
  let user = req.body;
  // Generate a salt and hash the password
  const salt = crypto.randomBytes(32).toString('hex');
  crypto.pbkdf2(user.password, salt, 100000, 64, 'sha512', (err, derivedKey) => {
    if (err) { //problem computing digest, like hash function not available
     reject("Error: " + err);
    }

    const hashedPassword = derivedKey.toString('hex');

    let newUser = {
      "id": users.length + 1,
      "first_name": user.first_name,
      "last_name": user.last_name,
      "username": user.username,
      "avatar": user.avatar,
      "salt": salt,
      "password": hashedPassword
    }

    users.push(newUser);

    let userFollows = {
      "userId": newUser.id,
      "following": []
    }

    follows[newUser.id] = userFollows;

    res.json(newUser);
  });
});

module.exports = apiRouter;