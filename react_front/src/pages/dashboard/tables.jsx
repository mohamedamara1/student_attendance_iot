import { useState, useEffect } from "react";
import mqtt from "precompiled-mqtt";

import {
  Card,
  CardHeader,
  CardBody,
  Typography,
  Avatar,
  Chip,
  Tooltip,
  Progress,
} from "@material-tailwind/react";

import { lessonsTableData, lessonsTableColumns, attendancesTableColumns, attendancesTableData } from "@/data";
import Table from "@/widgets/table";

export function Tables() {
  const [attendances, setAttendances] = useState([]);
  const [lessons, setLessons] = useState([]);
  const teacherId= localStorage.getItem("teacherId");
  const [selectedLessonId, setSelectedLessonId] = useState();
  const [mqttStatus, setMqttStatus] = useState("Connecting...");

  async function fetchLessonsTableData() {
    try {
      const response = await fetch(`http://192.168.1.8/lessons/findByTeacherId?teacherId=${teacherId}`);
      const data = await response.json();
      setLessons(data);
    } catch (error) {
      console.error(error);
    }
  }

  useEffect(() => {
    fetchLessonsTableData();
  }, []);

  async function handleLessonClick(lessonId) {
    console.log(lessonId);
    setSelectedLessonId(lessonId);
    try {
      const response = await fetch(`http://192.168.1.8/attendances/findByLessonId?lessonId=${lessonId}`);
      const filteredData = await response.json();
      setAttendances(filteredData);
    } catch (error) {
      console.error(error);
    }
  }

  useEffect(() => {
    // Connect to MQTT broker
    const esp32Ip ="192.168.1.8"
    const mqttBrokerPort = "1883"
    const mosqittoURL = "mqtt://test.mosquitto.org:8080";
    const espBrokerUrl = "mqtt://" + esp32Ip + ":" +mqttBrokerPort


   // const client = mqtt.connect(URL);
    
    // Subscribe to presence/lessonId topic
   // const topic = `presence/${lessonId}`;
   const options = {
    port: 1883,
    protocol: 'ws'
  };
    const client = mqtt.connect(espBrokerUrl);
    const topic = "presence";
    client.on('connect', function () {
      console.log("connected")
      client.subscribe(topic, function (err) {
        if (!err) {
          client.publish(topic, 'Hello mqtt')
          console.log("subscribed");
        }
      })
    })

  }, []);

  const handleConnect = () => {
    // Connect to MQTT broker
    // ...
    setMqttStatus("Connected");
  };

  const handleDisconnect = () => {
    // Disconnect from MQTT broker
    // ...
    setMqttStatus("Disconnected");
  };

 
  return (
    <div className="mt-12 mb-8 flex flex-col gap-12">
      <div className="flex justify-between items-center">
        <h2 className="text-2xl font-bold">MQTT Status: {mqttStatus}</h2>
        <div className="flex gap-4">
          <button>Connect</button>
          <button >Disconnect</button>
        </div>
      </div>
      <Table
        data={attendances}
        setAttendances={setAttendances}
        columns={attendancesTableColumns}
      />
      <Table data={lessons} columns={lessonsTableColumns} handleRowClick={handleLessonClick} />
    </div>
  );
}

export default Tables;
