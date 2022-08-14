/* server.js
 * Author: Harrison Bergeron
 * Date: July 30, 2022
 * Course: CS-499-H6772 Computer Science Capstone 22EW6 
 * Description: This is a server-side Node.js script to handle incoming chat
 *     messages from the client and store them in an sql database. It also 
 *     serves the static webpages that make up the website. It responds to 
 *     requests from the client but also sends queries to a local SQL 
 *     server. It is the middleman between the SQL server and the client. */



/* Libraries
 *
 * https: node.js module for creating secure http servers
 * fs: node.js module for loading files (certificates) 
 * node-static: node.js addon for serving static webpages
 * mysql2: node.js addon for accessing a mysql server */
const https = require('https');
const fs = require('fs');
const nodeStatic = require('node-static');
const mysql = require('mysql2')


// Connect to the local mysql database with premade username and password
const sql = mysql.createConnection({
    host: 'localhost',
    port: 3306,
    user: 'nodejsserver',
    password: '93trkagcr23p',
});

// Initialize the mysql database
initDb();

/* Create a static webpage server host for static webpage serving. All other
 * traffic will be for database operations. */
const staticServer = new nodeStatic.Server('./site');

// Function to load SSL certificate and listen on port 8000
function beginServer() {
    // Load private key and certificate for secure HTTP
    const options = {
        key: fs.readFileSync('key.pem'),
        cert: fs.readFileSync('cert.pem')
    };
    // Create the https server with request response callback
    https.createServer(options, function(req, res) {
        let data = ''
        // While the HTTP body is incomplete, build it with 'data' events
        req.on('data', function(chunk) {
            data += chunk;
        });
        // HTTP body is loaded, process data from client
        req.on('end', function() {
            // If request is a JSON POST request, client sent chat message
            if (req.method === 'POST' && 
                req.headers['content-type'] === 'application/json') {
                
                // Convert JSON string to object
                json = JSON.parse(data);
                
                /* Use prepared statement to use ip address data securely.
                 * Query to find the latest message ID sent by this IP and
                 * pass results to INSERT callback to insert this message
                 * as the successor. */
                sql.query('SELECT MAX(msg_id) AS x FROM ' + tableName + 
                    ' WHERE ip_addr = ?', [req.socket.remoteAddress], 
                    addMessageToDb(req.socket.remoteAddress, json.message));
                
                // Respond to the client
                res.end();
                
            // Request was not a JSON POST request, serve static webpages
            } else {
                staticServer.serve(req, res);
            }
        });
    // Server will listen on port 8000
    }).listen(8000);
}

// Database and table names that will be used/created
const dbName = 'chat_data';
const tableName = 'messages';

/* The following functions are for managing SQL queries and are meant to be
 * chained in the order they appear. These are callbacks that will be executed
 * in a deeply nested asynchronous series so that SQL queries happen in order
 * and wait for previous queries to finish. */

// Function that starts the callback chain
function initDb() {
    // Show databases available, pass the results to showDbs()
    sql.query('SHOW DATABASES', showDbs);
}

function showDbs(err, results) {
    let hasDb = false;
    // Set hasDb to true if databases listed include dbName as a database
    results.forEach(function(result) {
        if (result['Database'] === dbName)
            hasDb = true;
    });
    if (!hasDb) {
        // Create the database dbName if it doesn't exist
        sql.query('CREATE DATABASE ' + dbName, createDb);
    } else {
        // Database dbName exists, use it
        sql.query('USE ' + dbName, useDb);
    }
}

function createDb(err, results) {
    // Database didn't exist but was created, use it
    sql.query('USE ' + dbName, useDb);
}

function useDb(err, results) {
    // Show tables available, pass results to showTables
    sql.query('SHOW TABLES', showTables);
}

function showTables(err, results) {
    let hasTable = false;
    // Set hasTable to true if tables listed include tableName
    results.forEach(function(result) {
        if (result['Tables_in_' + dbName] === tableName)
            hasTable = true;
    });
    if (!hasTable) {
        /* Create the table tableName if it doesn't exist.
         * ip_addr: ip address of client user
         * msg_id: chronological index of this message
         * msg_time: date and time message was sent
         * msg: chat text that was sent by client user */
        sql.query('CREATE TABLE ' + tableName + 
            '(ip_addr VARCHAR(50), msg_id INT, ' +
            'msg_time DATETIME, msg VARCHAR(2000))',
            createTable1);
    } else {
        // Database and table exist, start the server
        beginServer();
    }
}

function createTable1(err, results) {
    // Table didn't exist but was created, start the server
    beginServer();
}


function addMessageToDb(ip, message) {
    /* Called after SELECT MAX query.
     * Actually inserts message into sql database. */
    function maxMessageId(err, results) {
        let max;
        // Find max msg_id so newest message has incremented index
        results.forEach(function(result) {
            max = result.x;
        });
        // If no messages exist, this is the first (index 0)
        if (max === null)
            max = -1;
        max++;
        // Use prepared statement with '?' to insert message data securely
        sql.query('INSERT INTO ' + tableName + ' VALUES ' + 
            '(?, ?, NOW(), ?)', [ip, max, message]);
    }
    
    return maxMessageId;
}