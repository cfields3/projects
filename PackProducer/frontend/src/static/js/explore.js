import api from './APIClient.js'

const postsList = document.getElementById('posts-list');

function sortByDate(a, b) {
  const dateA = new Date(a.post_datetime);
  const dateB = new Date(b.post_datetime);

  if (dateA > dateB)
    return -1;
  else if (dateB < dateA)
    return 1;
  else
    return 0;
};

api.getPosts().then(posts => {
  posts.sort(sortByDate);
  
  posts.forEach(post => {
    postsList.append(createPostHTML(post));
  });
});

function createPostHTML(post) {
  const container = document.createElement('div');
  container.classList.add('post');

  const title = document.createElement('h3');
  title.innerHTML = post.title;

  const postInfo = document.createElement('p');
  const d = new Date(post.post_datetime);
  let datetime = d.toLocaleString('en-US', {
    dateStyle: 'medium',
    timeStyle: 'short',
  });
  const c = new Date();
  let timeDifference = c - d;
  let timeDifferenceMinutes = Math.round(timeDifference / (1000 * 60));
  let timeDifferenceHours = Math.round(timeDifferenceMinutes / 24);
  let timeDifferenceDays = Math.round(timeDifferenceHours / 60);
  if (timeDifferenceDays == 0) {
    if (timeDifferenceHours == 0) {
      if (timeDifferenceMinutes == 0) {
        datetime = "just now";
      } else {
        datetime = timeDifferenceMinutes + ((timeDifferenceMinutes == 1) ? " minute ago" : " minutes ago");
      }
    } else {
      datetime = timeDifferenceHours + ((timeDifferenceHours == 1) ? " hour ago" : " hours ago");
    }
  }
  api.getUserById(post.authorId).then((user) => {
    postInfo.innerHTML = "last updated " + datetime +  " by " + user.username;
  });

  const postDescription = document.createElement('p');
  postDescription.innerHTML = post.description;

  const postLink = document.createElement('a');
  postLink.classList.add('post-link')
  postLink.setAttribute('href', '/event?id=' + post.id);
  postLink.innerHTML = 'Read More';  

  container.append(title, postInfo, postDescription, postLink);
  return container;
}