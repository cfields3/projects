import api from './APIClient.js';

// Get id from URL
const query = window.location.search;
let parameters = new URLSearchParams(query);
let id = parameters.get('id');

if (!id)
  document.location = '/error';

const title = document.getElementById('title');
const author = document.getElementById('author');
const tags = document.getElementById('tags');
const description = document.getElementById('description');
const location = document.getElementById('location');
const datetime = document.getElementById('datetime');

const interestedHeader = document.getElementById('interested');
const contactLink = document.getElementById('contact-link');
const editLink = document.getElementById('edit-link');

api.getPostById(id).then(post => {
  document.title = post.title + " - Pack Producer"

  title.innerHTML = post.title;

  api.getUserById(post.authorId).then(user => {
    api.getCurrentUser().then (currentUser => {
      if (user.id == currentUser.id) {
        author.innerHTML = "<em>event posted by you.</em>";
        editLink.setAttribute('href', '/editPost?id=' + post.id);
        editLink.removeAttribute('hidden');
      } else {
        author.innerHTML = "<em>event posted by " + user.username + ".</em>";
        contactLink.setAttribute('href', '/dm?id=' + user.id);
        contactLink.removeAttribute('hidden');
        interestedHeader.removeAttribute('hidden');
      }
    });
    
  });

  description.innerHTML = post.description;
  location.innerHTML = "<b>Where:</b> " + post.location;
  let postDate = new Date(post.event_datetime).toLocaleString('en-US', {
    dateStyle: 'medium',
    timeStyle: 'short',
  });
  datetime.innerHTML = "<b>When:</b> " + postDate;
}).catch(res => {
  document.location = '/error';
});
