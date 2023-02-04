const express = require('express');
const router = express.Router();

const cookieParser = require('cookie-parser');
router.use(cookieParser());

router.use(express.json());

const UserDAO = require('./data/UserDAO');
const PostDAO = require('./data/PostDAO');
const MessageDAO = require('./data/MessageDAO');
const TagDAO = require('./data/TagDAO');
const {TokenMiddleware, generateToken, removeToken} = require('./middleware/TokenMiddleware');
const { editUser } = require('./data/UserDAO');


/////////////////////////
// USER AUTHENTICATION //
/////////////////////////

// Logging in
router.post('/users/login', (req, res) => {
  if (req.body.username  && req.body.password) {
    UserDAO.getUserByCredentials(req.body.username, req.body.password).then(user => {
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

// Logging out
router.post('/users/logout', (req, res) => {
  removeToken(req, res);

  res.json({success: true});
});

// Get the current logged in user
router.get('/users/current', TokenMiddleware, (req, res) => {
  res.json(req.user);
});

// Add a new user
router.post('/users', (req, res) => {
  let newUser = req.body;
  console.log(newUser);
  UserDAO.createUser(newUser);
  res.json(newUser);
});

// Get a specific user by id
router.get('/users/:userId', TokenMiddleware, (req, res) => {
  UserDAO.getUserById(req.params.userId).then(user => {
    res.json(user);
  });
});

// Edit an account
router.put('/users', TokenMiddleware, (req, res) => {
  let editedUser = req.body;
  UserDAO.editUser(editedUser).then(user => {
    res.json(user);
  });
});

/////////////
// POSTING //
/////////////

// Get all posts
router.get('/posts', TokenMiddleware, (req,  res) => {
  PostDAO.getPosts().then(posts => {
    res.json(posts);
  });
});

// Create a new post
router.post('/posts', TokenMiddleware, (req, res) => {
  let newPost = req.body;
  PostDAO.createPost(newPost).then(post => {
    res.json(post);
  });
});

// Get posts created by a specific user
router.get('/posts/users/:userId', TokenMiddleware, (req, res) => {
  PostDAO.getPostsByUser(req.params.userId).then(posts => {
    res.json(posts);
  });
});

// Get a specific post by ID
router.get('/posts/:postId', TokenMiddleware, (req, res) => {
  PostDAO.getPostById(req.params.postId).then(post => {
    res.json(post);
  });
});

// Edit a Post
router.put('/posts', TokenMiddleware, (req, res) => {
  let editedPost = req.body;
  PostDAO.editPost(editedPost).then(post => {
    res.json(post);
  });
});

// Delete a Post
router.delete('/posts/:postId', TokenMiddleware, (req, res) => {
  PostDAO.deletePostById(req.params.postId).then(post => {
    res.json(post);
  });
});

//////////////
// MESSAGES //
//////////////

// Get all messages currently in the system
router.get('/messages', TokenMiddleware, (req, res) => {
  MessageDAO.getMessages().then(messages => {
    res.json(messages);
  });
});

// Get all messages sent by a particular user
router.get('/messages/sender/:senderId', TokenMiddleware, (req, res) => {
  MessageDAO.getMessagesBySender(req.params.senderId).then(messages => {
    res.json(messages);
  });
});

// Get all messages recieved by a particular user
router.get('/messages/reciever/:recieverId', TokenMiddleware, (req, res) => {
  MessageDAO.getMessagesByRecieiver(req.params.recieverId).then(messages => {
    res.json(messages);
  });
});

// Get messages sent and recieved by particular users
router.get('/messages/direct/:senderId/:recieverId', TokenMiddleware, (req, res) => {
  MessageDAO.getDirectMessages(req.params.senderId, req.params.recieverId).then(messages => {
    res.json(messages);
  });
});

// Send a message to a particular user
router.post('/messages', TokenMiddleware, (req, res) => {
  let newMessage = req.body;
  MessageDAO.createMessage(newMessage).then(message => {
    res.json(message);
  });
});

module.exports = router;