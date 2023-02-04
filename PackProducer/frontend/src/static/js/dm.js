import api from "./APIClient.js";

let recipientId;
let senderId;

api.getCurrentUser().then(user => {
  // Get id from URL
  const query = window.location.search;
  let parameters = new URLSearchParams(query);
  recipientId = parameters.get('id');
  senderId = user.id;

  if (!recipientId || recipientId == user.id)
    document.location = '/error';

  const messageInfo = document.getElementById('message-info');

  api.getUserById(recipientId).then(recipient => {
    messageInfo.innerHTML = 'Direct Message - ' + recipient.username;
    document.title = "Direct Message - " + recipient.username + " - Pack Producer"
  }).catch(res => {
    document.location = '/error';
  });

  const messageStream = document.getElementById('message-stream');

  api.getDirectMessages(senderId, recipientId).then(sentMessages => {
    api.getDirectMessages(recipientId, senderId).then(recievedMessages => {
      let allMessages = sentMessages.concat(recievedMessages);

      if (allMessages.length == 0) {
        const noMessagesAlert = document.createElement('h4');
        noMessagesAlert.innerHTML = "You don't have any messages yet. Start the conversation now!";
        noMessagesAlert.classList.add('no-info-alert');
        messageStream.append(noMessagesAlert);
      }

      allMessages.sort(sortByDate);

      allMessages.forEach(message => {
        messageStream.append(createMessageHTML(message));
      });
    });
  });

});

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

function createMessageHTML(message) {
  const container = document.createElement('div');
  container.classList.add('message');

  if (message.senderId == senderId) {
    container.classList.add('sent');
  } else {
    container.classList.add('recieved');
  }

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

const messageInput = document.getElementById('message-input');
const sendButton = document.getElementById('message-send');

const currentDatetime = new Date().toISOString();

function addMessage() {
  api.createMessage(senderId, recipientId, messageInput.value, currentDatetime);
  window.location.reload();
}

sendButton.addEventListener('click', (btn) => {
  addMessage();
});

messageInput.addEventListener('keydown', (input) => {
  if (!(input.key == 'Enter' && input.shiftKey)) {
    if (input.key == 'Enter') {
      addMessage();
    }
  }
});