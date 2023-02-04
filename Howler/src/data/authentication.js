const crypto = require('crypto');

let users = require('./users.json');

module.exports = {
  /* USERS */
  getUserByCredentials: (username, password) => {
    return new Promise((resolve, reject) => {
      const user = users.find(user => user.username == username);
      if (user) { // we found our user
        crypto.pbkdf2(password, user.salt, 100000, 64, 'sha512', (err, derivedKey) => {
          if (err) { //problem computing digest, like hash function not available
            reject("Error: " + err);
          }

          const digest = derivedKey.toString('hex');
          if (user.password == digest) {
            resolve(getFilteredUser(user));
          }
          else {
            reject("Invalid username or password");
          }
        });
      }
      else { // if no user with provided username
        reject("No such user");
      }
    });

  },

}

function getFilteredUser(user) {
  return {
    "id": user.id,
    "first_name": user.first_name,
    "last_name": user.last_name,
    "username": user.username,
    "avatar": user.avatar
  }
}