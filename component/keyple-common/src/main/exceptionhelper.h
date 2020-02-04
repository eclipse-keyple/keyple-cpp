/********************************************************************************
* Copyright (c) 2018 Calypso Networks Association https://www.calypsonet-asso.org/
*
* See the NOTICE file(s) distributed with this work for additional information regarding copyright
* ownership.
*
* This program and the accompanying materials are made available under the terms of the Eclipse
* Public License 2.0 which is available at http://www.eclipse.org/legal/epl-2.0
*
* SPDX-License-Identifier: EPL-2.0
********************************************************************************/

#pragma once

#include <string>

/* Common */
#include "Exception.h"

namespace keyple {
namespace common {

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

class IllegalStateException : public Exception {
  private:
    std::string msg;

  public:
    IllegalStateException(const std::string &message = "") : Exception(message)
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

class FileNotFoundException : public Exception {
  private:
    std::string msg;

  public:
    FileNotFoundException(const std::string &message = "") : Exception(message)
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

class NumberFormatException : public std::exception {
  private:
    std::string msg;

  public:
    NumberFormatException(const std::string &message = "") : msg(message)
    {
    }

    const char *what()
    {
        return msg.c_str();
    }
};

class IndexOutOfBoundException : public std::exception {
private:
  std::string msg;

public:
  IndexOutOfBoundException(const std::string &message = "") : msg(message)
  {
  }

  const char *what()
  {
      return msg.c_str();
  }
};

class AssertionError : public std::exception {
private:
  std::string msg;

public:
  AssertionError(const std::string &message = "") : msg(message)
  {
  }

  const char *what()
  {
      return msg.c_str();
  }
};

}
}

