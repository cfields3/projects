import api from './APIClient.js';

const signupButton = document.getElementById('signup-button');
const firstNameInput = document.getElementById('firstname-input');
const lastNameInput = document.getElementById('lastname-input');
const emailInput = document.getElementById('email-input');
const usernameInput = document.getElementById('username-input');
const passwordInput = document.getElementById('password-input');
const locationInput = document.getElementById('location-input');
const avatarInput = document.getElementById('avatar-input');

function signupUser() {
  let avatarAddress = null;
  if (avatarInput.files) {
    let filename = avatarInput.files[0].name;
    avatarAddress = 'uploads/' + Date.now().toString().substring(0, 10) + '.' + filename.split('.')[1];
  }
  
  api.createUser(firstNameInput.value, lastNameInput.value, usernameInput.value, passwordInput.value, emailInput.value, locationInput.value, avatarAddress)
  .then(res => {
    api.logIn(usernameInput.value, passwordInput.value)
    .then(res => {
      document.location = "/";
    })
  });
}

signupButton.addEventListener('click', (btn) => {
  signupUser();
});