import api from './APIClient.js';

const header = document.getElementById('greeting');
const postsFeed = document.getElementById('posts-feed');
const messagesFeed = document.getElementById('messages-feed');

function sortPostsByDate(a, b) {
  const dateA = new Date(a.post_datetime);
  const dateB = new Date(b.post_datetime);

  if (dateA > dateB)
    return -1;
  else if (dateB < dateA)
    return 1;
  else
    return 0;
};

function sortMessagesByDate(a, b) {
  const dateA = new Date(a.datetime);
  const dateB = new Date(b.datetime);

  if (dateA > dateB)
    return -1;
  else if (dateB < dateA)
    return 1;
  else
    return 0;
};

api.getCurrentUser().then(user => {
  header.innerHTML = "Welcome Back, " + user.first_name + "!";

  api.getPostsByUser(user.id).then(posts => {
    posts.sort(sortPostsByDate);
    
    posts.forEach(post => {
      postsFeed.append(createPostHTML(post));
    });

    if (posts.length == 0) {
      const noPostsAlert = document.createElement('h4');
      noPostsAlert.innerHTML = "Navigate to the Post tab to make your first one!";
      noPostsAlert.classList.add('no-info-alert');
      postsFeed.append(noPostsAlert);
    }
  });

  api.getMessagesByReciever(user.id).then(messages => {
    if (messages.length == 0) {
      const noMessagesAlert = document.createElement('h4');
      noMessagesAlert.innerHTML = "No messages have been sent to you yet. :(";
      noMessagesAlert.classList.add('no-info-alert');
      messagesFeed.append(noMessagesAlert);
    }

    messages.sort(sortMessagesByDate);

    messages.forEach(message => {
      messagesFeed.append(createMessageHTML(message));
    });
  });
});

function createPostHTML(post) {
  const container = document.createElement('div');
  container.classList.add('post');

  const title = document.createElement('h3');
  title.innerHTML = post.title;

  const postInfo = document.createElement('p');
  const d = new Date(post.post_datetime);
  let datetime = d.toLocaleString('en-US', {
    dateStyle: 'medium',
    timeStyle: 'short',
  });
  const c = new Date();
  let timeDifference = c - d;
  let timeDifferenceMinutes = Math.round(timeDifference / (1000 * 60));
  let timeDifferenceHours = Math.round(timeDifferenceMinutes / 24);
  let timeDifferenceDays = Math.round(timeDifferenceHours / 60);
  if (timeDifferenceDays == 0) {
    if (timeDifferenceHours == 0) {
      if (timeDifferenceMinutes == 0) {
        datetime = "just now";
      } else {
        datetime = timeDifferenceMinutes + ((timeDifferenceMinutes == 1) ? " minute ago" : " minutes ago");
      }
    } else {
      datetime = timeDifferenceHours + ((timeDifferenceHours == 1) ? " hour ago" : " hours ago");
    }
  }
  api.getUserById(post.authorId).then((user) => {
    postInfo.innerHTML = "last updated " + datetime;
  });

  const postDescription = document.createElement('p');
  postDescription.innerHTML = post.description;

  const postLink = document.createElement('a');
  postLink.classList.add('post-link')
  postLink.setAttribute('href', '/event?id=' + post.id);
  postLink.innerHTML = 'Read More';  

  container.append(title, postInfo, postDescription, postLink);
  return container;
}

function createMessageHTML(message) {
  const container = document.createElement('a');
  container.classList.add('message');

  container.setAttribute('href', '/dm?id=' + message.senderId);

  const avatar = document.createElement('img');
  const username = document.createElement('span');
  username.classList.add('account-name');
  const messageContents = document.createElement('span');
  messageContents.classList.add('message-contents');

  api.getUserById(message.senderId).then(user => {
    if (user.avatar) {
      avatar.setAttribute('src', user.avatar);
    } else {
      avatar.setAttribute('src', 'img/blank-profile-pic.webp');
    }
    username.innerHTML = user.username;
    messageContents.innerHTML = message.text;
  });

  container.append(avatar, username, messageContents);
  return container;
}