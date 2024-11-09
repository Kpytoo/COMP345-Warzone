//
// Created by william on 05/11/24.
//

#ifndef COMP345_WARZONE_LOGGINGOBSERVER_H
#define COMP345_WARZONE_LOGGINGOBSERVER_H

#include <string>
#include <vector>

/**
 * Interface for loggable objects. Classes that implement ILoggable
 * must provide a stringToLog() method, which returns a string
 * representing the log entry for the object.
 */
class ILoggable {
public:
    /**
     * Pure virtual function that should be implemented by any loggable
     * class. Returns a string that will be logged.
     * @return A string representing the log entry for the loggable object.
     */
    virtual std::string stringToLog() const = 0;
};

/**
 * Observer base class for implementing the Observer pattern.
 * Observers must implement the update() method, which is called
 * when a Subject notifies them of an event.
 */
class Observer {
public:
    /**
     * Pure virtual function that should be implemented by any loggable
     * class. Returns a string that will be logged.
     * @return A string representing the log entry for the loggable object.
     */
    virtual void update(ILoggable* loggable) = 0;
};

/**
 * Subject base class for implementing the Observer pattern.
 * Manages a list of Observer objects and notifies them of events.
 */
class Subject {
private:
    std::vector<Observer*> observers; ///< List of observers attached to this Subject.

public:
    /**
     * Attaches an Observer to this Subject.
     * @param observer A pointer to the Observer to attach.
     */
    void attach(Observer* observer);

    /**
     * Detaches an Observer from this Subject.
     * @param observer A pointer to the Observer to detach.
     */
    void detach(Observer* observer);

    /**
     * Notifies all attached Observers of an event by calling their
     * update() method.
     * @param loggable A pointer to the ILoggable object containing the
     *                 information to log.
     */
    void notify(ILoggable* loggable);
};

/**
 * Concrete Observer class that logs events to a file.
 * Implements the update() method to write log entries to
 * "gamelog.txt" whenever an event occurs.
 */
class LogObserver : public Observer {
public:
    LogObserver();

    /**
     * Called by a Subject to log an event. Writes the output of the
     * loggable's stringToLog() method to the "gamelog.txt" file.
     * @param loggable A pointer to the ILoggable object containing the
     *                 information to log.
     */
    void update(ILoggable* loggable) override;
};

#endif //COMP345_WARZONE_LOGGINGOBSERVER_H
