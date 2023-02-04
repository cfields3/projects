const crypto = require('crypto');

module.exports = class {
  #passwordHash;
  #salt;

  constructor(data) {
    this.id = data.usr_id;
    this.first_name = data.usr_first_name;
    this.last_name = data.usr_last_name;
    this.username = data.usr_username;
    this.email = data.usr_email;
    this.avatar = data.usr_avatar;
    this.location = data.usr_location;
    this.#salt = data.usr_salt;
    this.#passwordHash = data.usr_password;
  }

  validatePassword(password) {
    return new Promise((resolve, reject) => {
      crypto.pbkdf2(password, this.#salt, 100000, 64, 'sha512', (err, derivedKey) => {
        if (err) { //problem computing digest, like hash function not available
         reject("Error: " + err);
        }

        const digest = derivedKey.toString('hex');
        if (this.#passwordHash == digest) {
          resolve(this);
        }
        else {
          reject("Invalid username or password");
        }
      });
    });
  }

  toJSON() {
    return {
      id: this.id,
      first_name: this.first_name,
      last_name: this.last_name,
      username: this.username,
      email: this.email,
      avatar: this.avatar,
      location: this.location
    }
  }
};