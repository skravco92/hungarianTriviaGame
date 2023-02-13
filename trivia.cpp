#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

// Class to store the question and answer details
class Question
{
public:
  int questionId;
  std::string question;
  std::string opt_a;
  std::string opt_b;
  std::string opt_c;
  std::string opt_d;
  std::string answer;
};

// Class to handle the trivia game logic
class TriviaGame
{
private:
  std::vector<Question> questions;
  sql::Connection *conn;

public:
  TriviaGame()
  {
    try
    {
      // Connect to the database
      sql::Driver *driver = get_driver_instance();
      conn = driver->connect("localhost", "root", "Password123#@!");
      conn->setSchema("games");

      // Retrieve all the questions from the "trivia" table
      sql::PreparedStatement *pstmt = conn->prepareStatement("SELECT * FROM trivia");
      sql::ResultSet *res = pstmt->executeQuery();
      while (res->next())
      {
        Question
            q;
        q.questionId = res->getInt("questionId");
        q.question = res->getString("question");
        q.opt_a = res->getString("opt_a");
        q.opt_b = res->getString("opt_b");
        q.opt_c = res->getString("opt_c");
        q.opt_d = res->getString("opt_d");
        q.answer = res->getString("answer");
        questions.push_back(q);
      }
    }
    catch (const std::exception &e)
    {
      std::cerr << "Error: " << e.what() << std::endl;
      exit(1);
    }
  }

  // Method to play the trivia game
  void play()
  {
    // Shuffle the questions
    std::srand(unsigned(std::time(0)));
    std::random_shuffle(questions.begin(), questions.end());
    // Ask the questions
    int score = 0;
    for (const Question &q : questions)
    {
      // Display the question
      std::cout << "" << std::endl;
      std::cout << q.question << std::endl;
      std::cout << "" << std::endl;
      std::cout << "A. " << q.opt_a << std::endl;
      std::cout << "B. " << q.opt_b << std::endl;
      std::cout << "C. " << q.opt_c << std::endl;
      std::cout << "D. " << q.opt_d << std::endl;
      std::cout << "" << std::endl;
      // Get the player's response
      std::string response;
      bool validInput = false;
      while (!validInput)
      {
        std::cout << "Your answer: ";
        try
        {
          std::getline(std::cin, response);
          if (response == q.opt_a || response == q.opt_b || response == q.opt_c || response == q.opt_d)
          {
            // The response is valid
            validInput = true;
          }
          else
          {
            // The response is invalid
            std::cout << "Invalid option, please try again." << std::endl;
          }
        }
        catch (const std::exception &e)
        {
          // An error occurred
          std::cerr << "Error: " << e.what() << std::endl;
          response = "";
        }
      }

      // Check if the answer is correct
      if (response.compare(q.answer) == 0)
      {
        // The answer is correct
        std::cout << "Correct!" << std::endl;
        ++score;
      }
      else
      {
        // The answer is incorrect
        std::cout << "Wrong. " << q.answer << " is a right one!" << std::endl;
      }
    }

    // Print the final score
    std::cout << "You scored " << score << " out of " << questions.size() << "." << std::endl;
  }
};

int main()
{
  TriviaGame game;
  game.play();
  return 0;
}
