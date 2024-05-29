const express = require('express');
const path = require('path');
const open = require('open');
const { checkUserPrivileges } = require('./build/Release/user_privileges');

const app = express();
const PORT = 3000;

app.use(express.static(path.join(__dirname, 'public')));
app.use(express.urlencoded({ extended: true }));

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

app.post('/check', (req, res) => {
    const userName = req.body.username;
    const privilege = checkUserPrivileges(userName);
    res.redirect(`/result.html?username=${userName}&privilege=${privilege}`);
});

app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
    open(`http://localhost:${PORT}`);
});
