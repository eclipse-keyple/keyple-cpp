#pragma once

#include <string>
#include <sstream>
#include <iostream>

/* Common */
#include "Exception.h"

class IOException : public Exception {
  private:
    std::string msg;

  public:
    IOException(const std::string &message = "") : Exception(message)
    {
    }

    const char *what()
    {
        return msg.c_str();
    }
};

class SecurityException : public std::exception {
  private:
    std::string msg;

  public:
    SecurityException(const std::string &message = "") : msg(message)
    {
    }

    const char *what()
    {
        return msg.c_str();
    }
};

class NoSuchElementException : public std::exception {
  private:
    std::string msg;

  public:
    NoSuchElementException(const std::string &message = "") : msg(message)
    {
    }

    const char *what()
    {
        return msg.c_str();
    }
};

class IllegalAccessException : public std::exception {
  private:
    std::string msg;

  public:
    IllegalAccessException(const std::string &message = "") : msg(message)
    {
    }

    const char *what()
    {
        return msg.c_str();
    }
};

class InvocationTargetException : public std::exception {
  private:
    std::string msg;

  public:
    InvocationTargetException(const std::string &message = "") : msg(message)
    {
    }

    const char *what()
    {
        return msg.c_str();
    }
};

class NoSuchMethodException : public std::exception {
  private:
    std::string msg;

  public:
    NoSuchMethodException(const std::string &message = "") : msg(message)
    {
    }

    const char *what()
    {
        return msg.c_str();
    }
};

class ClassNotFoundException : public std::exception {
  private:
    std::string msg;

  public:
    ClassNotFoundException(const std::string &message = "") : msg(message)
    {
    }

    const char *what()
    {
        return msg.c_str();
    }
};

class NameNotFoundException : public std::exception {
  private:
    std::string msg;

  public:
    NameNotFoundException(const std::string &message = "") : msg(message)
    {
    }

    const char *what()
    {
        return msg.c_str();
    }
};

class IllegalStateException : public std::exception {
  private:
    std::string msg;

  public:
    IllegalStateException(const std::string &message = "") : msg(message)
    {
    }

    const char *what()
    {
        return msg.c_str();
    }
};

class IllegalArgumentException : public std::exception {
  private:
    std::string msg;

  public:
    IllegalArgumentException(const std::string &message = "") : msg(message)
    {
    }

    const char *what()
    {
        return msg.c_str();
    }
};

/*
class InterruptedException : public std::exception
{
private:
    std::string msg;

public:
    InterruptedException(const std::string& message = "") : msg(message)
    {
    }

    const char * what()
    {
        return msg.c_str();
    }
};
*/

class InstantiationException : public std::exception {
  private:
    std::string msg;

  public:
    InstantiationException(const std::string &message = "") : msg(message)
    {
    }

    const char *what()
    {
        return msg.c_str();
    }
};

/*
class CardException : public std::exception
{
private:
    std::string msg;

public:
    CardException(const std::string& message = "") : msg(message)
    {
    }

    const char * what()
    {
        return msg.c_str();
    }
};
*/

class JsonParseException : public std::exception {
  private:
    std::string msg;

  public:
    JsonParseException(const std::string &message = "") : msg(message)
    {
    }

    const char *what()
    {
        return msg.c_str();
    }
};

class FileNotFoundException : public std::exception {
  private:
    std::string msg;

  public:
    FileNotFoundException(const std::string &message = "") : msg(message)
    {
    }

    const char *what()
    {
        return msg.c_str();
    }
};

class URISyntaxException : public std::exception {
  private:
    std::string msg;

  public:
    URISyntaxException(const std::string &message = "") : msg(message)
    {
    }

    const char *what()
    {
        return msg.c_str();
    }
};

class PCSCException : public std::exception {
  private:
    std::string msg;

  public:
      long code;

    PCSCException(const std::string &message = "") : msg(message)
    {
    }

    const char *what()
    {
        return msg.c_str();
    }
};

class CardNotPresentException : public std::exception {
  private:
    std::string msg;

  public:
    CardNotPresentException(const std::string &message = "") : msg(message)
    {
    }

    const char *what()
    {
        return msg.c_str();
    }
};
