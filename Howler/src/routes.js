const express = require('express');
const router = express.Router();

const apiRouter = require('./APIRoutes');
const frontendRouter = require('./FrontendRoutes');

router.use(frontendRouter);
router.use('/api', apiRouter);


module.exports = router;