#ifndef __SELECT_HPP__
#define __SELECT_HPP__

#include <cstring>

class Select
{
public:
    virtual ~Select() = default;

    // Return true if the specified row should be selected.
    virtual bool select(const Spreadsheet* sheet, int row) const = 0;
};

// A common type of criterion for selection is to perform a comparison based on
// the contents of one column.  This class contains contains the logic needed
// for dealing with columns. Note that this class is also an abstract base
// class, derived from Select.  It introduces a new select function (taking just
// a string) and implements the original interface in terms of this.  Derived
// classes need only implement the new select function.  You may choose to
// derive from Select or Select_Column at your convenience.
class Select_Column: public Select
{
protected:
    int column;
public:
    Select_Column(const Spreadsheet* sheet, const std::string& name)
    {
        column = sheet->get_column_by_name(name);
    }

    virtual bool select(const Spreadsheet* sheet, int row) const
    {
        return select(sheet->cell_data(row, column));
    }

    // Derived classes can instead implement this simpler interface.
    virtual bool select(const std::string& s) const = 0;
};

class Select_And: public Select
{
    public:
    Select* first;
    Select* second;
    Select_And(Select* select1, Select* select2)
    {
        first = select1;
        second = select2;
    }
    ~Select_And()
    {
    delete first;
    delete second;
    }
    virtual bool select(const Spreadsheet* sheet, int row)
    {
        if ((first->select(sheet, row)==true) && (second->select(sheet, row)==true))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

class Select_Contains: public Select_Column
{
    public:
    std::string data;

    Select_Contains(const Spreadsheet* sheet, const std::string columnName, const std::string item):Select_Column(sheet, columnName)
    {
        data = item;
    }
    ~Select_Contains()
    {}
    virtual bool select(const std::string& s) const
    {
        if ( s.find(data) != std::string::npos)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

};

class Select_Not: public Select
{
    public:
    Select* argument;
    Select_Not(Select* input)
    {
        argument = input;
    }
    ~Select_Not()
     {
      delete argument;
     }

    virtual bool select(const Spreadsheet* sheet, int row)
    {
        if (argument->select(sheet, row) == false)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

class Select_Or: public Select
{
    public:
    Select* first;
    Select* second;
    Select_Or(Select* select1, Select* select2)
    {
        first = select1;
        second = select2;
    }
    ~Select_Or()
     {
      delete first;
      delete second;
     }

    virtual bool select(const Spreadsheet* sheet, int row)
    {
        if ((first->select(sheet, row)==true) || (second->select(sheet, row)==true))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};
#endif //__SELECT_HPP__
