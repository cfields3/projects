import api from "./APIClient.js";

const postEdit = document.getElementById('edit-post');
const postDelete = document.getElementById('delete-post');

const titleInput = document.getElementById('title-input');
const descriptionInput = document.getElementById('description-input');
// Tags/Categories
const locationInput = document.getElementById('location-input');
const dateInput = document.getElementById('date-input');

// Get id from URL
const query = window.location.search;
let parameters = new URLSearchParams(query);
let id = parameters.get('id');

if (!id)
  document.location = '/error';

api.getPostById(id).then(post => {
  api.getCurrentUser().then(author => {
    if (author.id != post.authorId)
      document.location = '/error';
  });
  titleInput.value = post.title;
  descriptionInput.value = post.description;
  locationInput.value = post.location;
  let date = post.event_datetime.substring(0, post.event_datetime.indexOf("T") + 6);
  dateInput.value = date;
}).catch(err => {
  document.location = '/error';
});

postEdit.addEventListener('click', (btn) => {
  api.getCurrentUser().then(author => {
    const postDateTime = new Date(dateInput.value).toISOString();
    const currentDatetime = new Date().toISOString();
    api.editPost(id, author.id, titleInput.value, descriptionInput.value, locationInput.value, postDateTime, currentDatetime);
    document.location = '/explore';
  });
});

postDelete.addEventListener('click', (btn) => {
  api.deletePost(id).then(post => {
    document.location = '/explore';
  });
});