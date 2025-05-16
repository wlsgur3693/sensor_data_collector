#--컴파일러 변수----
CXX = g++

#--플래그 및 라이브러리 설정----
LIBS = -ljsoncpp -lpthread -lssl -lcrypto -lpaho-mqttpp3 -lpaho-mqtt3c
CXXFLAGS = -Wall -Wextra -g -O1 -std=c++17

#--폴더 설정----
CONFIG_DIR          = Config
SENSOR_DATA_DIR     = sensor_data_collector
QUEUE_DIR           = queue
ENCODER_DIR         = encoder
SIGN_DIR            = sign
MQTT_DIR            = mqtt

#--소스 파일 및 오브젝트 파일 설정----
SRC = Logger.cpp \
      $(CONFIG_DIR)/Config.cpp \
      $(SENSOR_DATA_DIR)/SensorDataCollector.cpp \
      $(QUEUE_DIR)/DataQueue.cpp \
      $(ENCODER_DIR)/XOREncoder.cpp \
      $(SIGN_DIR)/Signer.cpp \
      $(MQTT_DIR)/MQTTSender.cpp

OBJ = Logger.o \
      $(CONFIG_DIR)/Config.o \
      $(SENSOR_DATA_DIR)/SensorDataCollector.o \
      $(QUEUE_DIR)/DataQueue.o \
      $(ENCODER_DIR)/XOREncoder.o \
      $(SIGN_DIR)/Signer.o \
      $(MQTT_DIR)/MQTTSender.o

#--기본 타겟----
all: Logger

#--타겟 파일을 생성----
Logger: $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LIBS)

#--개별 소스 파일을 오브젝트 파일로 컴파일하는 규칙----
%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS) -I./$(CONFIG_DIR) -I./$(SENSOR_DATA_DIR) -I./$(QUEUE_DIR) -I./$(ENCODER_DIR) -I./$(SIGN_DIR) -I./$(MQTT_DIR)

#--clean 규칙----
clean:
	rm -f $(OBJ) Logger
