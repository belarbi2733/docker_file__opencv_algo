FROM amine2733/new-algo1
#RUN apt-get update && add-apt-repository --yes ppa:xqms/opencv-nonfree && apt-get update && apt-get install --yes libopencv-features2d-dev libopencv-nonfree*
#COPY sift.cpp /root/sift.cpp
#COPY surf.cpp /root/surf.cpp
#COPY histo_cou.cpp /root/histo_cou.cpp
#COPY histo_hsv.cpp /root/histo_hsv.cpp
#COPY blur.cpp /root/blur.cpp
#COPY gaussian.cpp /root/gaussian.cpp
#COPY median.cpp /root/median.cpp
#COPY bilateral.cpp /root/bilateral.cpp
#COPY harris.cpp /root/harris.cpp
#COPY canny.cpp /root/canny.cpp
#COPY countours.cpp /root/countours.cpp
#COPY laplacien.cpp /root/laplacien.cpp
#COPY water.cpp /root/water.cpp
#COPY indexation.cpp /root/indexation.cpp
#COPY recherche.cpp /root/recherche.cpp
#COPY wang /root/wang
COPY darknet /root/darknet
RUN cd /root/darknet && make
#RUN cd /root && g++ sift.cpp -o sift `pkg-config --cflags --libs opencv`
#RUN cd /root && g++ surf.cpp -o surf `pkg-config --cflags --libs opencv`
#RUN cd /root && g++ histo_cou.cpp -o histo_cou `pkg-config --cflags --libs opencv`
#RUN cd /root && g++ histo_hsv.cpp -o histo_hsv `pkg-config --cflags --libs opencv`
#RUN cd /root && g++ blur.cpp -o blur `pkg-config --cflags --libs opencv`
#RUN cd /root && g++ gaussian.cpp -o gaussian `pkg-config --cflags --libs opencv`
#RUN cd /root && g++ median.cpp -o median `pkg-config --cflags --libs opencv`
#RUN cd /root && g++ bilateral.cpp -o bilateral `pkg-config --cflags --libs opencv`
#RUN cd /root && g++ harris.cpp -o harris `pkg-config --cflags --libs opencv`
#RUN cd /root && g++ canny.cpp -o canny `pkg-config --cflags --libs opencv`
#RUN cd /root && g++ countours.cpp -o countours `pkg-config --cflags --libs opencv`
#RUN cd /root && g++ laplacien.cpp -o laplacien `pkg-config --cflags --libs opencv`
#RUN cd /root && g++ water.cpp -o water `pkg-config --cflags --libs opencv`
#RUN cd /root && g++ indexation.cpp -o indexation `pkg-config --cflags --libs opencv`
#RUN cd /root && g++ recherche.cpp -o recherche `pkg-config --cflags --libs opencv`
#RUN ls /root/wang
#RUN /root/indexation SIFT
#RUN /root/indexation SURF
#RUN /root/indexation HISTO
COPY script.sh /root/script.sh
RUN sudo chmod +x /root/script.sh
CMD cd /home && /root/script.sh
