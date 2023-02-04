import api from './APIClient.js';

const avatar = document.getElementById('avatar');
const name = document.getElementById('name');
const username = document.getElementById('username');
const email = document.getElementById('email');
const location = document.getElementById('location');

api.getCurrentUser().then(user => {
  if (user.avatar) {
    avatar.setAttribute('src', user.avatar);
  } else {
    avatar.setAttribute('src', 'img/blank-profile-pic.webp');
  }
  name.innerHTML = user.first_name + " " + user.last_name;
  username.innerHTML = user.username;
  email.innerHTML = user.email;
  if (user.location) {
    location.innerHTML = user.location;
  } else {
    location.innerHTML = "Not Specified"
  }
});

const editButton = document.getElementById('edit-button');
const accountEditor = document.getElementById('edit-account');

editButton.addEventListener('click', (btn) => {
  accountEditor.removeAttribute('hidden');
  editButton.toggleAttribute('hidden'); 
  alert('Fill out the fields cooresponding to the account information you would like to update (you do not need to fill out every field).\n\nPlease note that you will be logged out upon filling out the form.');
});

const firstNameInput = document.getElementById('edit-firstname');
const lastNameInput = document.getElementById('edit-lastname');
const usernameInput = document.getElementById('edit-username');
const emailInput = document.getElementById('edit-email');
const locationInput = document.getElementById('edit-location');

const editSubmit = document.getElementById('submit-edit');

editSubmit.addEventListener('click', (btn) => {
  api.getCurrentUser().then(user => {
    if (firstNameInput.value != '')
      user.first_name = firstNameInput.value;
    if (lastNameInput.value != '')
      user.last_name = lastNameInput.value;
    if (usernameInput.value != '')
      user.username = usernameInput.value;
    if (emailInput.value != '')
      user.email = emailInput.value;
    if (locationInput.value != '')
      user.location = locationInput.value;
    api.editUser(user.id, user.first_name, user.last_name, user.username, user.email, user.location, user.avatar);
    api.logOut();
    alert('Changes successfully made! Please log back in to see edits.');
    document.location = '/login';
  });
});



const logoutButton = document.getElementById('logout-button');

function logout() {
  api.logOut()
  .then(res => {
    document.location = "/login";
  });
}

logoutButton.addEventListener('click', (btn) => {
  logout();
});