import HTTPClient from "./HTTPClient.js";

const API_BASE = '/api';

export default {
  logIn: (username, password) => {
    let data = {
      username: username,
      password: password
    }
    return HTTPClient.post(API_BASE + '/users/login', data);
  },

  logOut: () => {
    return HTTPClient.post(API_BASE + '/users/logout', {});
  },

  getCurrentUser: () => {
    return HTTPClient.get(API_BASE + '/users/current');
  },

  createUser: (first_name, last_name, username, password, email, location, avatar) => {
    let data = {
      first_name: first_name,
      last_name: last_name,
      username: username,
      email: email,
      avatar: avatar,
      location: location,
      password: password
    }
    return HTTPClient.post(API_BASE + '/users', data);
  },

  getPosts: () => {
    return HTTPClient.get(API_BASE + '/posts');
  },

  getPostById: (id) => {
    return HTTPClient.get(API_BASE + '/posts/' + id);
  },

  createPost: (authorId, title, description, location, eventDatetime, postDatetime) => {
    let data = {
      authorId: authorId,
      title: title,
      description: description,
      location: location,
      eventDatetime: eventDatetime,
      postDatetime: postDatetime
    };
    return HTTPClient.post(API_BASE + '/posts', data);
  },

  getUserById: (id) => {
    return HTTPClient.get(API_BASE + '/users/' + id);
  },

  editUser: (id, first_name, last_name, username, email, location, avatar) => {
    let data = {
      id: id,
      first_name: first_name,
      last_name: last_name,
      username: username,
      email: email,
      location: location,
      avatar: avatar
    };
    return HTTPClient.put(API_BASE + '/users', data);
  },

  getDirectMessages: (sender_id, reciever_id) => {
    return HTTPClient.get(API_BASE + '/messages/direct/' + sender_id + "/" + reciever_id);
  },

  createMessage: (senderId, recieverId, text, datetime) => {
    let data = {
      senderId: senderId,
      recieverId: recieverId,
      text: text,
      datetime: datetime
    };
    return HTTPClient.post(API_BASE + '/messages', data);
  },

  getPostsByUser: (userId) => {
    return HTTPClient.get(API_BASE + '/posts/users/' + userId);
  },

  editPost: (id, authorId, title, description, location, eventDatetime, postDatetime) => {
    let data = {
      id: id,
      authorId: authorId,
      title: title,
      description: description,
      location: location,
      eventDatetime: eventDatetime,
      postDatetime: postDatetime
    };
    return HTTPClient.put(API_BASE + '/posts', data);
  },

  deletePost: (id) => {
    return HTTPClient.delete(API_BASE + '/posts/' + id);
  },

  getMessagesByReciever: (recieverId) => {
    return HTTPClient.get(API_BASE + '/messages/reciever/' + recieverId);
  },

  getMessagesBySender: (senderId) => {
    return HTTPClient.get(API_BASE + '/messages/sender/' + senderId);
  }
};
