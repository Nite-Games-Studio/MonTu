#ifndef FPSMANAGER_H
#define FPSMANAGER_H


class FPSManager
{
    public:
        FPSManager();
        virtual ~FPSManager();

		void start();
		int elapsedTime();
    protected:

    private:
		int stime;
};

#endif // FPSMANAGER_H
