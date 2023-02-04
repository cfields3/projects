const express = require('express');
const multer = require('multer');
const frontendRouter = express.Router();

const path = require('path');

const uploads_dir = path.join(__dirname, '../static/uploads/');

// Give filepath and extension for file uploads
const storage = multer.diskStorage({
  destination: function (req, file, cb) {
      cb(null, uploads_dir);
  },
  filename: function (req, file, cb) {
      cb(null, Date.now().toString().substring(0, 10) + path.extname(file.originalname)); // Appending extension
  }
});

// Specify method for file uploads
const upload = multer({ storage: storage });

const html_dir = path.join(__dirname ,'../templates/');

frontendRouter.get('/', (req, res) => {
  res.sendFile(`${html_dir}index.html`);
});

frontendRouter.post('/', upload.single('imageFile'), (req, res) => {
  res.sendFile(`${html_dir}index.html`);
});

frontendRouter.get('/user', (req,  res) => {
  res.sendFile(`${html_dir}user.html`);
});

frontendRouter.get('/login', (req,  res) => {
  res.sendFile(`${html_dir}login.html`);
});

frontendRouter.get('/signup', (req, res) => {
  res.sendFile(`${html_dir}signup.html`);
});



module.exports = frontendRouter;