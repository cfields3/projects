import api from './APIClient.js';

const rightHeaderBlock = document.querySelector('.right-block');

api.getCurrentUser().then(currentUser => {
  rightHeaderBlock.append(createCurrentUserHTML(currentUser));
}).catch(error => {
  window.location.href = '/login';
})

function createCurrentUserHTML(currentUser) {
  const profile = document.createElement('a');
  profile.href = '/user/?id=' + currentUser.id;
  profile.classList.add('profile');

  const currentUsername = document.createElement('p');
  currentUsername.innerHTML = '@' + currentUser.username;
  currentUsername.classList.add('current-username');

  const profilePicture = document.createElement('img');
  profilePicture.src = currentUser.avatar;

  profile.append(currentUsername, profilePicture);

  return profile;
}