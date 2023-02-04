const db = require('./DBConnection');
const Message = require('./models/Message');

function getMessages() {
  return db.query('SELECT * FROM message').then(({results}) => {
    return results.map(message => new Message(message)); ;
  });
}

function getMessagesBySender(senderId) {
  return db.query('SELECT * FROM message WHERE msg_senderId=?', [senderId]).then(({results}) => {
    return results.map(message => new Message(message)); ;
  });
}

function getMessagesByRecieiver(recieverId) {
  return db.query('SELECT * FROM message WHERE msg_recieverId=?', [recieverId]).then(({results}) => {
    return results.map(message => new Message(message)); ;
  });
}

function getDirectMessages(senderId, recieverId) {
  return db.query('SELECT * FROM message WHERE msg_senderId=? AND msg_recieverId=?', [senderId, recieverId]).then(({results}) => {
    return results.map(message => new Message(message)); ;
  });
}

function getMessageById(messageId) {
  return db.query('SELECT * FROM message WHERE msg_id=?', [messageId]).then(({results}) => {
    if(results[0])
      return new Message(results[0]);
  });
}

function createMessage(message) {
  return db.query('INSERT INTO message (`msg_senderId`, `msg_recieverId`, `msg_text`, `msg_datetime`) VALUES (?, ?, ?, ?)', [message.senderId, message.recieverId, message.text, message.datetime]).then(({results}) => {
    return getMessageById(results.insertId);
  });
}


module.exports = {
  getMessages: getMessages,
  getMessagesBySender: getMessagesBySender,
  getMessagesByRecieiver: getMessagesByRecieiver,
  getDirectMessages: getDirectMessages,
  getMessageById: getMessageById,
  createMessage: createMessage,
};

