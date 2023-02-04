import HTTPClient from "./HTTPClient.js";

const API_BASE = '/api';

export default {
  getFollowedHowls: () => {
    return HTTPClient.get(API_BASE + '/users/current/follows/howls');
  },

  getPostedHowls: (userId) => {
    return HTTPClient.get(API_BASE + '/users/' + userId + '/howls');
  },

  getUserById: (userId) => {
    return HTTPClient.get(API_BASE + '/users/' + userId);
  },

  getCurrentUser: () => {
    return HTTPClient.get(API_BASE + '/users/current');
  },

  getFollowedUsers: (userId) => {
    return HTTPClient.get(API_BASE + '/users/' + userId + '/follows');
  },

  postHowl: (newHowl) => {
    return HTTPClient.post(API_BASE + '/howls', newHowl);
  },

  getHowlCount: () => {
    return HTTPClient.get(API_BASE + '/howls/count');
  },

  followUser: (userId) => {
    return HTTPClient.get(API_BASE + '/users/' + userId + '/follow');
  },

  unfollowUser: (userId) => {
    return HTTPClient.get(API_BASE + '/users/' + userId + '/unfollow');
  },

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

  createUser: (first_name, last_name, username, password, avatar) => {
    let data = {
      first_name: first_name,
      last_name: last_name,
      username: username,
      password: password,
      avatar: avatar
    }
    return HTTPClient.post(API_BASE + '/users/', data);
  },
};
