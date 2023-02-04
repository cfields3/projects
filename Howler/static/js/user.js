import api from './APIClient.js';
import howlBuilder from './howls.js';

// Get id from URL
const query = window.location.search;
let parameters = new URLSearchParams(query);
let id = parameters.get('id');

const userProfile = document.querySelector('.user-profile');
const avatar = document.getElementById('user-avatar');
const name = document.getElementById('user-name');
const username = document.getElementById('user-username');
const followBtn = document.getElementById('follow-button');
const logOutBtn = document.getElementById('log-out-button');

api.getUserById(id).then(user => {
  avatar.src = user.avatar;
  name.innerHTML = user.first_name + " " + user.last_name;
  username.innerHTML = "@" + user.username;
  api.getCurrentUser().then(currentUser => {
    document.title = user.first_name + " " + user.last_name + " - Howler";
    if (currentUser.id != user.id) {
      logOutBtn.style.display = 'none';
      api.getFollowedUsers(currentUser.id).then(followedUsers => {
        if (followedUsers.includes(user.id)) {
          followBtn.innerHTML = 'Unfollow';
        } else {
          followBtn.innerHTML = 'Follow';
        }
      });
    } else {
      followBtn.style.display = 'none';
    }
  });
});

api.getFollowedUsers(id).then(followedUsers => {
  const followsList = document.getElementById('follows-list');
  const followsPreview = document.getElementById('follows-preview');
  const firstUser = followedUsers.shift();
  api.getUserById(firstUser).then(user => {
    followsPreview.append(createFollowHTML(user));
  });
  followedUsers.forEach(userId => {
    api.getUserById(userId).then(user => {
      followsList.append(createFollowHTML(user));
    });
  });
});

api.getPostedHowls(id).then(howls => {
  const howlsList = document.getElementById('user-howls-list');
  howls.forEach(howl => {
    howlsList.append(howlBuilder.createHowlHTML(howl));
  });
});

logOutBtn.addEventListener('click', () => {
  api.logOut().then(res => {
    window.location.href = '/login';
  });
});

followBtn.addEventListener('click', () => {
  api.getCurrentUser().then(currentUser => {
    api.getFollowedUsers(currentUser.id).then(followedUsers => {
      if (followedUsers.includes(parseInt(id, 10))) {
        api.unfollowUser(parseInt(id, 10));
        followBtn.innerHTML = 'Follow';
      } else {
        api.followUser(parseInt(id, 10));
        followBtn.innerHTML = 'Unfollow';
      }
    });
  }); 
});

function createFollowHTML(user) {
  const container = document.createElement('a');
  container.classList.add('followed-user');
  container.href = '/user/?id=' + user.id;

  const avatar = document.createElement('img');
  avatar.src = user.avatar;

  const name = document.createElement('h3');
  name.classList.add('followed-user-name');
  name.innerHTML = user.first_name + ' ' + user.last_name;

  const username = document.createElement('p');
  username.classList.add('followed-user-username');
  username.innerHTML = '@' + user.username;

  container.append(avatar, name, username);
  return container;
}