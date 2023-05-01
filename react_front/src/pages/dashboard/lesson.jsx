import React, { useState } from 'react';
import { Card, CardBody, Button, Alert, CardHeader} from "@material-tailwind/react";


const LessonPage = (props) => {
    const [attendance, setAttendance] = useState(false);
    const { courseName, teacherName, date, time } = props;
  
    const handleAttendance = () => {
      setAttendance(true);
    };

    return (
            <div className="flex justify-center pt-8">
              <Card>
                <CardHeader color="indigo" contentPosition="left">
                  <h2 className="text-white text-2xl">Course Name: {courseName}</h2>
                </CardHeader>
                <CardBody>
                  <div className="flex flex-col items-center">
                    <h6 className="text-lg">Teacher: {teacherName}</h6>
                    <h1 className="text-2xl font-medium mt-2">Date: {date}</h1>
                    <h2 className="text-lg font-medium mt-2">Time: {time}</h2>
          
                    {!attendance && (
                      <Button
                        color="indigo"
                        buttonType="filled"
                        size="lg"
                        block={true}
                        className="mt-8"
                        onClick={handleAttendance}
                      >
                        Mark Attendance
                      </Button>
                    )}
          
                    {attendance && (
                      <Alert type="success" onClose={() => setAttendance(false)} className="mt-8">
                        Attendance Marked!
                      </Alert>
                    )}
                  </div>
                </CardBody>
                <CardBody></CardBody>
              </Card>
            </div>
          );   
};

export default LessonPage;
