import api from './APIClient.js'

const contacts = document.getElementById('contacts');

let userList = [];

api.getCurrentUser().then(currentUser => {
  api.getMessagesByReciever(currentUser.id).then(messages => {
    messages.sort(sortByDate);
    messages.forEach(message => {
      if (!userList.includes(message.senderId)) {
        userList.push(message.senderId);
      }
    });
    userList.forEach(senderId => {
      contacts.append(createContactHTML(senderId));
    });
    if (userList.length == 0) {
      const noContactsAlert = document.createElement('h4');
      noContactsAlert.innerHTML = "To start the conversation, explore posts on the Explore tab and contact the user on the page for that post (under Read More).";
      noContactsAlert.classList.add('no-info-alert');
      contacts.append(noContactsAlert);
    }
  });
});

function createContactHTML(senderId) {
  const container = document.createElement('a');
  container.classList.add('contact');
  container.setAttribute('href', '/dm?id=' + senderId);

  const avatar = document.createElement('img');
  const contactPreview = document.createElement('div');
  contactPreview.classList.add('contact-preview');

  const name = document.createElement('p');
  name.classList.add('name');

  const messagePreview = document.createElement('p');
  messagePreview.classList.add('message-preview');

  contactPreview.append(name, messagePreview);
  container.append(avatar, contactPreview);

  api.getUserById(senderId).then(user => {
    if (user.avatar) {
      avatar.src = user.avatar;
    } else {
      avatar.src = 'img/blank-profile-pic.webp'
    }

    name.innerHTML = user.first_name + " " + user.last_name;
  });

  api.getCurrentUser().then(currentUser => {
    api.getDirectMessages(senderId, currentUser.id).then(sentMessages => {
      api.getDirectMessages(currentUser.id, senderId).then(recievedMessages => {
        let allMessages = sentMessages.concat(recievedMessages);
  
        allMessages.sort(sortByDate);

        messagePreview.innerHTML = allMessages[0].text.substring(0, 50);
        if (messagePreview.innerHTML[49] == " ") {
          messagePreview.innerHTML = messagePreview.innerHTML.substring(0, 49);
        }
        if (allMessages[0].text.charAt(50)) {
          messagePreview.innerHTML += "...";
        }
      });
    });
  });

  return container;
}

function sortByDate(a, b) {
  const dateA = new Date(a.datetime);
  const dateB = new Date(b.datetime);

  if (dateA > dateB)
    return -1;
  else if (dateB < dateA)
    return 1;
  else
    return 0;
};