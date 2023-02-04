import api from './APIClient.js';

export default {
  createHowlHTML: (howl) => {
    const container = document.createElement('div');
    container.classList.add('howl');

    const userAndTime = document.createElement('div');
    userAndTime.classList.add('user-and-time');

    const userLink = document.createElement('a');
    userLink.classList.add('user');

    const profilePicture = document.createElement('img');
    const name = document.createElement('h4');
    name.classList.add('name');
    const username = document.createElement('h4');
    username.classList.add('username');
    api.getUserById(howl.userId).then(user => {
      userLink.href = '/user/?id=' + user.id;
      profilePicture.src = user.avatar;
      name.innerHTML = user.first_name + " " + user.last_name;
      username.innerHTML = "@" + user.username;
    });
    userLink.append(profilePicture, name, username);
    userAndTime.append(userLink);

    const dateTime = document.createElement('p');
    const d = new Date(howl.datetime);
    dateTime.innerHTML = d.toLocaleString('en-US', {
      dateStyle: 'medium',
      timeStyle: 'short',
    });
    userAndTime.append(dateTime);

    container.append(userAndTime);

    const text = document.createElement('p');
    text.classList.add('text');
    text.innerHTML = howl.text;
    container.append(text);

    return container;
  },
}