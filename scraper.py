import requests
import mysql.connector
from bs4 import BeautifulSoup

# Define a header for the request to identify the user agent
header = {"User-Agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/109.0.0.0 Safari/537.36"}

# The URL from where the data is to be extracted
url = "https://psycatgames.com/hu/magazine/party-games/trivia-questions/"

# Make a GET request to the URL
response = requests.get(url)

# Create a BeautifulSoup object to parse the HTML content
soup = BeautifulSoup(response.content, "html.parser")

# Find the div element that contains the content to be extracted
n = soup.find("div", {"class": "content-wrapper"})

# Find all the questions
questions = n.find_all("h3")

# Connect to the database
conn = mysql.connector.connect(user='root', password='Password123#@!', host='localhost', database='games', charset='utf8mb4')

# Create a cursor for the database connection
cursor = conn.cursor()

# Loop through each question
for question in questions:
    # Extract the question text 
    question_text = question.text.encode('utf-8').decode('utf-8', 'ignore')

    # Find the corresponding ul element that contains the options
    options_container = question.find_next("ul")
    
    # Extract all li elements within the ul element, representing the options
    options = [option.text.encode('utf-8').decode('utf-8', 'ignore') for option in options_container.find_all("li")]

    # Store the extracted options in separate variables
    opt_a = options[0]
    opt_b = options[1]
    opt_c = options[2]
    opt_d = options[3]

    # Find the answer
    answer = question.find_next("h4").text.encode('utf-8').decode('utf-8', 'ignore')

    # Insert the data into the database using the cursor
    query = "INSERT INTO trivia (question, opt_a, opt_b, opt_c, opt_d, answer) VALUES (%s, %s, %s, %s, %s, %s)"
    cursor.execute(query, (question_text, opt_a, opt_b, opt_c, opt_d, answer))

# Commit the changes to the database
conn.commit()

# Close the cursor and connection
cursor.close()
conn.close()
