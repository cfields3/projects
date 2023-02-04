const db = require('./DBConnection');
const User = require('./models/User');
const crypto = require('crypto');

function getUserByCredentials(username, password) {
  return db.query('SELECT * FROM user WHERE usr_username=?', [username]).then(({results}) => {
    const user = new User(results[0]);
    if (user) { // we found our user
      return user.validatePassword(password);
    }
    else { // if no user with provided username
      throw new Error("No such user");
    }
  });
}

function getUserById(id) {
  return db.query('SELECT * FROM user WHERE usr_id=?', [id]).then(({results}) => {
    const user = new User(results[0]);
    if (user) {
      return user;
    }
    else {
      throw new Error("No such user");
    }
  });
}

function createUser(user) {
  // Generate a salt and hash the password
  const salt = crypto.randomBytes(32).toString('hex');
  console.log(salt);
  crypto.pbkdf2(user.password, salt, 100000, 64, 'sha512', (err, derivedKey) => {
    console.log('test');
    if (err) { //problem computing digest, like hash function not available
     reject("Error: " + err);
    }

    const digest = derivedKey.toString('hex');

    return db.query('INSERT INTO user (`usr_id`, `usr_first_name`, `usr_last_name`, `usr_username`, `usr_email`, `usr_avatar`, `usr_location`, `usr_salt`, `usr_password`) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)', [user.id, user.first_name, user.last_name, user.username, user.email, user.avatar, user.location, salt, digest]).then(({results}) => {
      return user;
    });

  });
};

function editUser(user) {
  return db.query('UPDATE user SET usr_first_name = ?, usr_last_name = ?, usr_username = ?, usr_email = ?, usr_avatar = ?, usr_location = ? WHERE usr_id = ?', [user.first_name, user.last_name, user.username, user.email, user.avatar, user.location, user.id]).then(({results}) => {
    return user;
  });
}


module.exports = {
  getUserByCredentials: getUserByCredentials,
  createUser: createUser,
  getUserById: getUserById,
  editUser: editUser
};
