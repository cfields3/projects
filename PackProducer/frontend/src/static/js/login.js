import api from './APIClient.js';

const loginButton = document.getElementById('login-button');
const usernameInput = document.getElementById('username-input');
const passwordInput = document.getElementById('password-input');

let foundError = false;

function loginUser() {
  api.logIn(usernameInput.value, passwordInput.value)
  .then(res => {
    document.location = "/";
  })
  .catch(error => {
    if (!foundError) {
      const errorDisplay = document.createElement('p')
      errorDisplay.innerHTML = 'Invalid Username or Password. Please Try Again.';
      errorDisplay.classList.add('error-display');
      const errorContainer = document.getElementById('error-container')
      errorContainer.append(errorDisplay);
      foundError = true;
    }
  })
}

loginButton.addEventListener('click', (btn) => {
  loginUser();
});

usernameInput.addEventListener('keydown', (input) => {
  if (input.key == 'Enter') {
    loginUser();
  }
});

passwordInput.addEventListener('keydown', (input) => {
  if (input.key == 'Enter') {
    loginUser();
  }
});