import api from "./APIClient.js";

const postSubmit = document.getElementById('submit-post');

const titleInput = document.getElementById('title-input');
const descriptionInput = document.getElementById('description-input');
// Tags/Categories
const locationInput = document.getElementById('location-input');
const dateInput = document.getElementById('date-input');

function makePost() {
  let errorMessage = '';
  if (titleInput.value == '')
    errorMessage += 'Enter a Title for the Post.\n';
  if (descriptionInput.value.length < 10)
    errorMessage += 'Enter a Description of at least 10 characters for the Post.\n';
  if (dateInput.value == '')
    errorMessage += 'Enter a Date for the Event';
  if (errorMessage.length != 0) {
    alert('Invalid Information Entered! Please Review the Following:\n\n' + errorMessage);
    return;
  } else {
    let confirmation = confirm("Post New Event?");
    if (!confirmation)
      return;
  }
  api.getCurrentUser().then(user => {
    const postDateTime = new Date(dateInput.value).toISOString();
    const currentDatetime = new Date().toISOString();
    api.createPost(user.id, titleInput.value, descriptionInput.value, locationInput.value, postDateTime, currentDatetime);
    document.location = '/explore';
  });
}

postSubmit.addEventListener('click', (btn) => {
   makePost(); 
});