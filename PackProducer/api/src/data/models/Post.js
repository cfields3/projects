module.exports = class {
  constructor(data) {
    this.id = data.pst_id;
    this.authorId = data.pst_authorId;
    this.title = data.pst_title;
    this.description = data.pst_description;
    this.location = data.pst_location;
    this.event_datetime = data.pst_event_datetime;
    this.post_datetime = data.pst_post_datetime;
  }
};