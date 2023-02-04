const express = require('express');
const cookieParser = require('cookie-parser');

const app = express();
const PORT = 3000;

app.use(express.static('static'));
app.use(cookieParser());

const routes = require('./src/routes');
app.use(routes);

// Listen for incoming connections on the specified port
app.listen(PORT, () => console.log(`Server listening on port: ${PORT}`));