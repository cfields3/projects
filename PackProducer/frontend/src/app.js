const express = require('express');
const multer = require('multer');

const app = express();
const PORT = process.env.PORT;

// Designate the static folder as serving static resources
app.use(express.static(__dirname + '/static'));

const path = require('path');
const html_dir = path.join(__dirname ,'/templates/');

// Give filepath and extension for file uploads
const storage = multer.diskStorage({
  destination: function (req, file, cb) {
      cb(null, __dirname + '/static/uploads/');
  },
  filename: function (req, file, cb) {
      cb(null, Date.now().toString().substring(0, 10) + path.extname(file.originalname)); // Appending extension
  }
});

// Specify method for file uploads
const upload = multer({ storage: storage });


app.get('/post', (req, res) => {
  res.sendFile(`${html_dir}post.html`);
});

app.get('/account', (req, res) => {
  res.sendFile(`${html_dir}account.html`);
});

app.get('/message', (req, res) => {
  res.sendFile(`${html_dir}message.html`);
});

app.get('/login', (req, res) => {
  res.sendFile(`${html_dir}login.html`);
});

app.get('/signup', (req, res) => {
  res.sendFile(`${html_dir}signup.html`);
});

app.get('/explore', (req, res) => {
  res.sendFile(`${html_dir}explore.html`);
});

app.get('/', (req, res) => {
  res.sendFile(`${html_dir}dashboard.html`);
});

app.post('/', upload.single('imageFile'), (req, res) => {
  res.sendFile(`${html_dir}dashboard.html`);
});

app.get('/welcome', (req, res) => {
  res.sendFile(`${html_dir}welcome.html`);
});

app.get('/event', (req, res) => {
  res.sendFile(`${html_dir}event.html`);
});

app.get('/dm', (req, res) => {
  res.sendFile(`${html_dir}dm.html`);
});

app.get('/editPost', (req, res) => {
  res.sendFile(`${html_dir}editPost.html`);
});

app.get('/error', (req, res) => {
  res.sendFile(`${html_dir}error.html`);
});

app.get('/offline', (req, res) => {
  res.sendFile(`${html_dir}offline.html`);
});

// As our server to listen for incoming connections
app.listen(PORT, () => console.log(`Server listening on port: ${PORT}`));