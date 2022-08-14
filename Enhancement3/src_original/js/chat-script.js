
let chatInput = document.getElementsByClassName("chat-input")[0];
let chatScroll = document.getElementsByClassName("chat-scroll")[0];
let chatMessages = document.getElementsByClassName("chat-messages")[0];
chatInput.addEventListener("keydown", function(event) {
    if (event.key === "Enter" && chatInput.value !== "") {
        chatMessages.insertAdjacentHTML("beforeend", 
            "<br>" + 
            "<div class=\"chat-name-right\"><span>You</span></div>" +
            "<hr class=\"chat-divider\">" +
            "<span class=\"chat-text\">" +
            chatInput.value +
            "</span>");
        chatInput.value = "";
        chatScroll.scrollTop = chatScroll.scrollHeight;
    }
});