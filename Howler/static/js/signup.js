import api from './APIClient.js';

const signupButton = document.getElementById('signup-button');
const firstNameInput = document.getElementById('firstname-input');
const lastNameInput = document.getElementById('lastname-input');
const usernameInput = document.getElementById('username-input');
const passwordInput = document.getElementById('password-input');
const confirmPasswordInput = document.getElementById('confirm-password-input');
const avatarInput = document.getElementById('avatar-input');

function signupUser() {
  let avatarAddress = null;
  if (avatarInput.files) {
    let filename = avatarInput.files[0].name;
    avatarAddress = '/uploads/' + Date.now().toString().substring(0, 10) + '.' + filename.split('.')[1];
  }

  if (passwordInput.value != confirmPasswordInput.value) {
    document.location = "/";
    console.log('test');
  }
  
  api.createUser(firstNameInput.value, lastNameInput.value, usernameInput.value, passwordInput.value, avatarAddress);
}

signupButton.addEventListener('click', (btn) => {
  signupUser();
});