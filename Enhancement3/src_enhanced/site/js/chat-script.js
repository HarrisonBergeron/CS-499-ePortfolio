/* chat-script.js
 * Author: Harrison Bergeron
 * Date: July 30, 2022
 * Course: CS-499-H6772 Computer Science Capstone 22EW6 
 * Description: This is a client-side script to add DOM elements to the window
 *     representing a chat interface whenever the user enters a chat message. 
 *     It also sends HTTP requests to the Node.js server when the user enters
 *     a chat message to inform the server to store the message in the 
 *     database. This script exists to provide client-side functionality to the
 *     browser when the user visits the website. */



/* Function to get elements on the page by their class name. If there is 
 * more than one, return the first. This function exists to reduce repetitive
 * code and provide a safeguard for out of bounds errors. */
function elementByClassName(className) {
    let elementList = document.getElementsByClassName(className);
    if (elementList.length === 0)
        return null;
    else
        return elementList[0];
}

// Input text field for chat
let chatInput = elementByClassName('chat-input');
// Scroll pane for chat, holds messages
let chatScroll = elementByClassName('chat-scroll');
// Messages in chat (text and usernames)
let chatMessages = elementByClassName('chat-messages');

// Limit the number of messages displayed on screen for performance
let numMessages = 1;
const maxMessages = 100; 

// If these elements don't exist, don't initialize chat feature
if (chatInput !== null && chatScroll !== null && chatMessages !== null) {

    // Add a key listener to detect when the user sends a message with 'enter'
    chatInput.addEventListener('keydown', function(event) {
        // If the user submits a non-empty string
        if (event.key === 'Enter' && chatInput.value !== '') {
            // Limit the input string to 2000 chars to prevent spam
            const chatInputStr = chatInput.value.substring(0, 2000);
            
            
            // Insert the new chat message using DOM style
            
            // Space separating this message from previous 
            let newline = document.createElement('br');
            // Text to distinguish your message from agent
            let nameText = document.createElement('span');
            nameText.textContent = 'You';
            // Holds the nameText span element
            let nameContainer = document.createElement('div');
            nameContainer.setAttribute('class', 'chat-name-right');
            nameContainer.appendChild(nameText);
            // Dividing line between username and message
            let divider = document.createElement('hr');
            divider.setAttribute('class', 'chat-divider');
            // Actual message sent
            let messageText = document.createElement('p');
            messageText.setAttribute('class', 'chat-text');
            messageText.textContent = chatInputStr;
            
            // Append chat message elements to chat messages's children
            chatMessages.insertAdjacentElement('beforeend', newline);
            chatMessages.insertAdjacentElement('beforeend', nameContainer);
            chatMessages.insertAdjacentElement('beforeend', divider);
            chatMessages.insertAdjacentElement('beforeend', messageText);
            
            // Scroll to the bottom of the chat box to show latest message
            chatScroll.scrollTop = chatScroll.scrollHeight;
        
            /* Create an HTTP request to send the message to the server
             * for storage */
            const req = new XMLHttpRequest();
            /* Use POST method to signal creation of new data on server. Async
             * is true to prevent waiting for server response. */
            req.open('POST', '/index.html', true);
            // We will send the message as JSON
            req.setRequestHeader('content-type', 'application/json');
            /* Send message as the body (JSON string). The server will receive
             * this data and hopefully store it in its local MySQL server. */
            req.send(JSON.stringify({
                message: chatInputStr
            }));
            
            // If there are too many messages on the screen, remove oldest ones
            numMessages++;
            while (numMessages > maxMessages) {
                // Each chat message takes up 4 elements
                for (let i = 0; i < 4; i++) {
                    chatMessages.removeChild(chatMessages.children[0]);
                }
                numMessages--;
            }
            
            // User hit enter, clear the text field for next message
            chatInput.value = '';
        }
    });

}