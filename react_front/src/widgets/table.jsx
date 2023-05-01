import React from "react";
import { useState } from "react";

import PropTypes from "prop-types";
import { Card, CardBody, CardHeader, Typography, Avatar, Chip } from "@material-tailwind/react";

function Table({ data, columns, handleRowClick }) {
  const [clickedRowIndex, setClickedRowIndex] = useState(null);

  return (
    <Card>
      <CardHeader variant="gradient" color="blue" className="mb-8 p-6">
        <Typography variant="h6" color="white">
          {columns.title}
        </Typography>
      </CardHeader>
      <CardBody className="overflow-x-scroll px-0 pt-0 pb-2">
        <table className="w-full min-w-[640px] table-auto">
          <thead>
            <tr>
              {columns.headers.map((header) => (
                <th key={header} className="border-b border-blue-gray-50 py-3 px-5 text-left">
                  <Typography
                    variant="small"
                    className="text-[11px] font-bold uppercase text-blue-gray-400"
                  >
                    {header}
                  </Typography>
                </th>
              ))}
            </tr>
          </thead>
          <tbody>
            {data.map((item, index) => (
              <tr key={index}  
              onClick={() => {handleRowClick(item.id); setClickedRowIndex(index);}}    
              className={`hover:bg-gray-100 ${clickedRowIndex === index ? "bg-blue-100" : ""} hover:bg-blue-50 transition duration-500 ease-in-out`
              }>
                  {columns.columns.map(({ accessor, render }) => (
                  <td
                    key={accessor}
                    className={`py-3 px-5 ${
                      index === data.length - 1 ? "" : "border-b border-blue-gray-50"
                    }`}
                  >
                    {render ? render(item[accessor]) : item[accessor]}
                  </td>
                ))}
              </tr>
            ))}
          </tbody>
        </table>
      </CardBody>
    </Card>
  );
}

Table.propTypes = {
  data: PropTypes.array.isRequired,
  columns: PropTypes.object.isRequired,
};

export default Table;
