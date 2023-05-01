import React from "react";
import {
  Typography,
  Card,
  CardHeader,
  IconButton,
  Menu,
  MenuHandler,
  MenuList,
  MenuItem
} from "@material-tailwind/react";
import {
  CheckIcon,
  EllipsisVerticalIcon,
} from "@heroicons/react/24/outline";


export function Home() {
  return (
    <div className="mt-12">
  
      <div className="mb-4 grid grid-cols-1 gap-6 xl:grid-cols-3">
        <Card className="overflow-hidden xl:col-span-2 mx-auto">
          <CardHeader
            floated={false}
            shadow={false}
            color="transparent"
            className="m-0 flex items-center justify-between p-6 text-center"
          >
              <Typography variant="h6" color="blue-gray" className="mb-1">
                Welcome user
              </Typography>
          </CardHeader>
        </Card>
      
      </div>
    </div>
  );
}

export default Home;
