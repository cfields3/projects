module.exports = class {
  constructor(data) {
    this.id = data.msg_id;
    this.senderId = data.msg_senderId;
    this.recipientId = data.msg_recipientId;
    this.text = data.msg_text;
    this.datetime = data.msg_datetime;
  }
};