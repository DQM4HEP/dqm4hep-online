  /// \file Parameter.h
/*
 *
 * Parameter.h header template automatically generated by a class generator
 * Creation date : mer. janv. 25 2017
 *
 * This file is part of DQM4HEP libraries.
 *
 * DQM4HEP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 *
 * DQM4HEP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DQM4HEP.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author Remi Ete
 * @copyright CNRS , IPNL
 */


#ifndef PARAMETER_H
#define PARAMETER_H

#include "dqm4hep/DQM4HEP.h"

namespace dqm4hep {

  namespace core {

    /**
     * Parameter template class
     */
    template <typename T>
    class Parameter
    {
    public:
      /**
       * Constructor
       */
      Parameter();

      /**
       * Constructor
       *
       * @param value an input value
       */
      Parameter(const T &value);

      /**
      * Copy Constructor
      *
      * @param parameter an input parameter
       */
      Parameter(const Parameter<T> &parameter);

      /**
       * Destructor
       */
      ~Parameter();

      /**
       * Get the parameter
       */
      const T &get() const;

      /**
       * Set the parameter
       *
       * @param value the value to copy
       */
      void set(const T &value);

      /**
       * Whether the parameter is initialized
       */
      bool isInitialized() const;

      /**
       * Reset the parameter
       */
      void reset();

      /**
       * Assignment operator with value
       *
       * @param value the value to assign
       */
      bool operator =(const T &value);

      /**
       * Assignment operator with parameter
       *
       * @param parameter the parameter to assign
       */
      bool operator =(const Parameter<T> &parameter);

    private:
      /**
       * Whether the value is valid (not inf or nan)
       *
       * @param  value the value to test
       */
      bool isValid(const T &value) const;

    private:
      T              m_value;             ///< The wrapped value
      bool           m_initialized;       ///< Whether the parameter is initialized
    };

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    /**
     * Parameter template class
     */
    template <typename T>
    class Parameter< std::vector<T> >
    {
    public:
      /**
       * Constructor
       */
      Parameter();

      /**
       * Constructor
       *
       * @param value an input value
       */
      Parameter(const std::vector<T> &value);

      /**
      * Copy Constructor
      *
      * @param parameter an input parameter
       */
      Parameter(const Parameter< std::vector<T> > &parameter);

      /**
       * Destructor
       */
      ~Parameter();

      /**
       * Get the parameter
       */
      const std::vector<T> &get() const;

      /**
       * Set the parameter
       *
       * @param value the value to copy
       */
      void set(const std::vector<T> &value);

      /**
       * Whether the parameter is initialized
       */
      bool isInitialized() const;

      /**
       * Reset the parameter
       */
      void reset();

      /**
       * Assignment operator with value
       *
       * @param value the value to assign
       */
      bool operator =(const std::vector<T> &value);

      /**
       * Assignment operator with parameter
       *
       * @param parameter the parameter to assign
       */
      bool operator =(const Parameter< std::vector<T> > &parameter);

    private:
      /**
       * Whether the value is valid (not inf or nan)
       *
       * @param  value the value to test
       */
      bool isValid(const std::vector<T> &value) const;

    private:
      std::vector<T>              m_value;             ///< The wrapped value
      bool                        m_initialized;       ///< Whether the parameter is initialized
    };

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    typedef Parameter<int>                                 IntParameter;
    typedef Parameter<unsigned int>                        UIntParameter;
    typedef Parameter<float>                               FloatParameter;
    typedef Parameter<double>                              DoubleParameter;
    typedef Parameter<bool>                                BoolParameter;
    typedef Parameter<std::string>                         StringParameter;
    typedef Parameter<std::vector<int> >                   IntVectorParameter;
    typedef Parameter<std::vector<unsigned int> >          UIntVectorParameter;
    typedef Parameter<std::vector<float> >                 FloatVectorParameter;
    typedef Parameter<std::vector<double> >                DoubleVectorParameter;
    typedef Parameter<std::vector<bool> >                  BoolVectorParameter;
    typedef Parameter<std::vector<std::string> >           StringVectorParameter;

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline Parameter<T>::Parameter() :
      m_value(0),
      m_initialized(false)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline Parameter<T>::Parameter(const T &value) :
      m_value(value),
      m_initialized(true)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline Parameter<T>::Parameter(const Parameter<T> &parameter) :
      m_initialized(parameter.m_initialized)
    {
      if(m_initialized)
        m_value = parameter.m_value;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline Parameter<T>::~Parameter()
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline const T &Parameter<T>::get() const
    {
      if(!m_initialized)
        throw StatusCodeException(STATUS_CODE_NOT_INITIALIZED);

      return m_value;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline void Parameter<T>::set(const T &value)
    {
      if(!this->isValid(value))
        throw StatusCodeException(STATUS_CODE_INVALID_PARAMETER);

      m_value = value;
      m_initialized = true;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline bool Parameter<T>::isInitialized() const
    {
      return m_initialized;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline void Parameter<T>::reset()
    {
      m_initialized = false;
      m_value = 0;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline bool Parameter<T>::operator =(const T &value)
    {
      this->set(value);
      return m_initialized;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline bool Parameter<T>::operator =(const Parameter<T> &parameter)
    {
      if(this == &parameter)
        return m_initialized;

      if(parameter.m_initialized)
      {
        this->set(parameter.m_value);
      }
      else
      {
        this->reset();
      }

      return m_initialized;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline bool Parameter<T>::isValid(const T &value) const
    {
      return !(IS_NAN(value) || IS_INF(value));
    }

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    // std::string specialization

    template <>
    inline Parameter<std::string>::Parameter() :
      m_value(""),
      m_initialized(false)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline void Parameter<std::string>::reset()
    {
      m_initialized = false;
      m_value.clear();
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline bool Parameter<std::string>::isValid(const std::string &value) const
    {
      return !value.empty();
    }

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------
    // vector specialization

    template <typename T>
    inline Parameter< std::vector<T> >::Parameter() :
      m_value(),
      m_initialized(false)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline Parameter< std::vector<T> >::Parameter(const std::vector<T> &value) :
      m_value(value),
      m_initialized(true)
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline Parameter< std::vector<T> >::Parameter(const Parameter< std::vector<T> > &parameter) :
      m_initialized(parameter.m_initialized)
    {
      if(m_initialized)
        m_value = parameter.m_value;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline Parameter< std::vector<T> >::~Parameter()
    {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline const std::vector<T> &Parameter< std::vector<T> >::get() const
    {
      if(!m_initialized)
        throw StatusCodeException(STATUS_CODE_NOT_INITIALIZED);

      return m_value;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline void Parameter< std::vector<T> >::set(const std::vector<T> &value)
    {
      if(!this->isValid(value))
        throw StatusCodeException(STATUS_CODE_INVALID_PARAMETER);

      m_value = value;
      m_initialized = true;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline bool Parameter< std::vector<T> >::isInitialized() const
    {
      return m_initialized;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline void Parameter< std::vector<T> >::reset()
    {
      m_initialized = false;
      m_value = 0;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline bool Parameter< std::vector<T> >::operator =(const std::vector<T> &value)
    {
      this->set(value);
      return m_initialized;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline bool Parameter< std::vector<T> >::operator =(const Parameter< std::vector<T> > &parameter)
    {
      if(this == &parameter)
        return m_initialized;

      if(parameter.m_initialized)
      {
        this->set(parameter.m_value);
      }
      else
      {
        this->reset();
      }

      return m_initialized;
    }

    //-------------------------------------------------------------------------------------------------

    template <typename T>
    inline bool Parameter< std::vector<T> >::isValid(const std::vector<T> &value) const
    {
      for(auto iter = value.begin(), endIter = value.end() ; endIter != iter ; ++iter)
        if( (IS_NAN(*iter) || IS_INF(*iter)) )
          return false;

      return true;
    }

    //-------------------------------------------------------------------------------------------------

    template <>
    inline bool Parameter< std::vector<std::string> >::isValid(const std::vector<std::string> &value) const
    {
      for(auto iter = value.begin(), endIter = value.end() ; endIter != iter ; ++iter)
        if( (*iter).empty() )
          return false;

      return true;
    }

  }

}

#endif  //  PARAMETER_H
