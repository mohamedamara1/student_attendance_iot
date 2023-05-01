import React from 'react';
import { Redirect, Route } from 'react-router-dom';

const ProtectedRoute = ({ role, roles = [], ...props }) => {
    return !roles.length || roles.includes(role) ? (
      <Route {...props} />
    ) : (
      <Redirect to=".." />
    );
  };
export default ProtectedRoute;
