const db = require('./DBConnection');
const Tag = require('./models/Tag');

function getTagByName(name) {
  return db.query('SELECT * FROM tag WHERE tag_name=?', [name]).then(({results}) => {
    if(results[0])
      return new Tag(results[0]);
  });
}

function getTagById(tagId) {
  return db.query('SELECT * FROM tag WHERE tag_id=?', [tagId]).then(({results}) => {
    if(results[0])
      return new Tag(results[0]);
  });
}


module.exports = {
  getTagByName: getTagByName,
  getTagById: getTagById,
};

