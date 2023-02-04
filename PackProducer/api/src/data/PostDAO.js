const db = require('./DBConnection');
const Post = require('./models/Post');

function getPosts() {
  return db.query('SELECT * FROM post').then(({results}) => {
    return results.map(post => new Post(post)); ;
  });
}

function getPostsByUser(userId) {
  return db.query('SELECT * FROM post WHERE pst_authorId=?', [userId]).then(({results}) => {
    return results.map(post => new Post(post)); ;
  });
}

function getPostById(postId) {
  return db.query('SELECT * FROM post WHERE pst_id=?', [postId]).then(({results}) => {
    if(results[0])
      return new Post(results[0]);
  });
}

function createPost(post) {
  return db.query('INSERT INTO post (`pst_authorId`, `pst_title`, `pst_description`, `pst_location`, `pst_event_datetime`, `pst_post_datetime`) VALUES (?, ?, ?, ?, ?, ?)', [post.authorId, post.title, post.description, post.location, post.eventDatetime, post.postDatetime]).then(({results}) => {
    return getPostById(results.insertId);
  });
}

function addPostTag(post, tag) {
  return db.query('INSERT INTO post_tag (ptt_pst_id, ptt_tag_id) VALUES (?, ?)', [post.id, tag.id]).then(({results}) => {
    return;
  });
}

function editPost(post) {
  return db.query('UPDATE post SET pst_authorId = ?, pst_title = ?, pst_description = ?, pst_location = ?, pst_event_datetime = ?, pst_post_datetime = ? WHERE pst_id = ?', [post.authorId, post.title, post.description, post.location, post.eventDatetime, post.postDatetime, post.id]).then(({results}) => {
    return post;
  });
}

function deletePostById(id) {
  let deletedPost = getPostById(id);
  return db.query('DELETE FROM post WHERE pst_id = ?', [id]).then(({results}) => {
    return deletedPost;
  });
}


module.exports = {
  getPosts: getPosts,
  getPostsByUser: getPostsByUser,
  getPostById: getPostById,
  createPost: createPost,
  addPostTag: addPostTag,
  editPost: editPost,
  deletePostById: deletePostById
};

