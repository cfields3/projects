import api from './APIClient.js';
import howlBuilder from './howls.js';

const howlsList = document.getElementById('howls-list');

api.getFollowedHowls().then(howls => {
  howls.forEach(howl => {
    howlsList.append(howlBuilder.createHowlHTML(howl));
  });
});

// Get the form element
const form = document.querySelector('.post-box');
const textBox = document.getElementById('text-box');

// Add 'submit' event handler
form.addEventListener("submit", (event) => {
  event.preventDefault();

  api.getHowlCount().then(howlCount => {
    api.getCurrentUser().then(currentUser => {
      let datetime = new Date().getTime();
      let newHowl = {
        "id": howlCount + 1,
        "userId": currentUser.id,
        "datetime": datetime,
        "text": textBox.value,
      };
      textBox.value = '';
      api.postHowl(newHowl);
    });
  });

});