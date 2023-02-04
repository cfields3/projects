# Homework 5: Authentication with JWT

Assignment implementing a simple authenticated website with a small frontend and backend using JWTs. For this assignment, I chose to build upon my Howler application from the previous homework assignment. Navigate to the main html files for this assignment below:

- [Main Page (index.html)](templates/index.html)
- [Login Page (login.html)](templates/login.html)
- [User Page (user.html)](templates/user.html)

### Reflection:

1. An interesting challenge that I encountered when implementing JWT algorithms was using the crypto package to compute an HMAC. The most challenging part of this was creating an HMAC in proper base64url encoding so that the token would meet the requirements for a compliant JWT. I solved this issue by looking through lots of documentation for the crypto.createHmac() function in order to find how to be use it, including the option to have 'base64url' as an encoding option when a function for hmac.digest().

2. My implementation may be vulnerable to some sort of attempt to intercept the information sent from a user to the server and store a copy so that the attacker can log in as if they are the user. This could possibly be solved by adding some sort of salt to the JWT token so that it is a one-time authentication process and that information could not be used later, even if it was intercepted by an attacker. This could come with a trade-off where it would take more time and storage to send the information to the server.

### Updates Since Homework Deadline
- Extended features of Howler to include the ability to add users to the application.