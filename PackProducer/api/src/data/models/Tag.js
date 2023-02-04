module.exports = class {
  constructor(data) {
    this.id = data.tag_id;
    this.name = data.tag_name;
    this.icon = data.tag_icon;
  }
};