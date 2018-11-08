/**
 * Generic Observable class
 *
 * @param <T> Generic event
 *
 */

namespace keyple {
	namespace seproxy {
		namespace util {
			class Observable<T> {

  public
    interface Observer<T>
    {
        void update(T event);
    }

  private
    boolean changed = false;

  private
    final Collection<Observer<T>> observers;

  public
    Observable()
    {
        observers = new CopyOnWriteArrayList<Observer<T>>();
    }

  public
    void addObserver(final Observer<T> observer)
    {
        if (!observers.contains(observer))
        {
            observers.add(observer);
        }
    }

  public
    void removeObserver(final Observer<T> observer)
    {
        observers.remove(observer);
    }

  public
    void clearObservers()
    {
        this.observers.clear();
    }

  public
    void setChanged()
    {
        this.changed = true;
    }

  public
    void clearChanged()
    {
        this.changed = false;
    }

  public
    boolean hasChanged()
    {
        return this.changed;
    }

  public
    int countObservers()
    {
        return observers.size();
    }

  public
    void notifyObservers()
    {
        notifyObservers(null);
    }

  public
    void notifyObservers(final T event)
    {
        for (Observer<T> observer : observers)
        {
            observer.update(event); // the Observable is already present in the event
        }
    }
}