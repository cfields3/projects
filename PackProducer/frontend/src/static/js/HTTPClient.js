const handleError = (res) => {
  if(!res.ok) {
    if(res.status == 401) {
      throw new Error("Unauthenticated");
    }
    else {
      throw new Error(res.json().error)
    }
  }
  return res;
};

export default {
  get: (url) => {
    return fetch(url, {
      headers: {
      }
    }).then(handleError).then(res => {
      return res.json();
    });
  },

  post: (url, data) => {
    return fetch(url, {
      method: 'POST',
      body: JSON.stringify(data),
      headers: {
        'Content-Type': 'application/json',
      }
    }).then(handleError).then(res => {
      return res.json();
    });
  },

  put: (url, data) => {
    return fetch(url, {
      method: 'PUT',
      body: JSON.stringify(data),
      headers: {
        'Content-Type': 'application/json',
      }
    }).then(handleError).then(res => {
      return res.json();
    });

  },

  delete: (url) => {
    return fetch(url, {
      method: 'DELETE',
      headers: {
      }
    }).then(handleError).then(res => {
      return res.json();
    });
  },

};
