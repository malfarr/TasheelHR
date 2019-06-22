<?php
$type = $variables['type'];
?>

<p>Dear @[name],</p>

<?php if ($type == HRM_EMAIL_NOTIF_ASSET_ASSIGNED) { ?>
  <p>Please note that @[assigned_to_name] has been assigned a @[category] by @[assigned_by_name].</p>
  <p><strong>Asset details:</strong></p>
  <div><strong>Asset ID:</strong> @[asset_no]</div>
  <div><strong>Serial Number:</strong> @[serial_number]</div>
  <div><strong>Category:</strong> @[category]</div>
  <div><strong>Brand:</strong> @[brand]</div>
  <div><strong>Model:</strong> @[model]</div>
  <div><strong>Condition of asset upon assignment:</strong> @[assign_condition]</div>
<?php } ?>

<?php if ($type == HRM_EMAIL_NOTIF_ASSET_ASSIGNMENT_NOT_COMPLETED) { ?>  
  <p>Please be informed that the asset assignment process of @[assigned_to_name]'s @[category] is pending the upload of the appropriate asset assignment information.</p>
  <p><strong>Asset details:</strong></p>
  <div><strong>Assigned on:</strong> @[assign_date]</div>
  <div><strong>Assigned to:</strong> @[assigned_to_name]</div>
  <div><strong>Assigned by:</strong> @[assigned_by_name]</div>
  <div><strong>Asset Number:</strong> @[asset_no]</div>
  <div><strong>Serial Number:</strong> @[serial_number]</div>
  <div><strong>Category:</strong> @[category]</div>
  <div><strong>Brand:</strong> @[brand]</div>
  <div><strong>Model:</strong> @[model]</div>
<?php } ?>

<?php if ($type == HRM_EMAIL_NOTIF_ASSET_RETURN) { ?>  
  <p>Please be informed that the asset return process of @[assigned_to_name]'s @[category] is pending the upload of the appropriate asset return information.</p>
  <p><strong>Asset details:</strong></p>
  <div><strong>Returned on:</strong> @[return_date]</div>
  <div><strong>Assigned to:</strong> @[assigned_to_name]</div>
  <div><strong>Returned by:</strong> @[returned_by_name]</div>
  <div><strong>Asset Number:</strong> @[asset_no]</div>
  <div><strong>Serial Number:</strong> @[serial_number]</div>
  <div><strong>Category:</strong> @[category]</div>
  <div><strong>Brand:</strong> @[brand]</div>
  <div><strong>Model:</strong> @[model]</div>
  <div><strong>Condition of asset upon return:</strong> @[return_condition]</div>
<?php } ?>

<?php if ($type == HRM_EMAIL_NOTIF_ASSET_RETURN_NOT_COMPLETED) { ?>  
  <p>Please be informed that the asset return process of @[assigned_to_name]'s @[category] is pending the upload of the appropriate asset return information.</p>
  <p><strong>Asset details:</strong></p>
  <div><strong>Returned on:</strong> @[return_date]</div>
  <div><strong>Assigned to:</strong> @[assigned_to_name]</div>
  <div><strong>Returned by:</strong> @[returned_by_name]</div>
  <div><strong>Asset Number:</strong> @[asset_no]</div>
  <div><strong>Serial Number:</strong> @[serial_number]</div>
  <div><strong>Category:</strong> @[category]</div>
  <div><strong>Brand:</strong> @[brand]</div>
  <div><strong>Model:</strong> @[model]</div>
<?php } ?>
<p>
  Best Regards,<br /><br />
  @[organization_name]<br />
  @[department]
</p>
<br /><br />
<hr/>
<p class="footer-message">
  @[responsehrm_name] is a human resource management solution that is feature-rich, intuitive and provides an essential HR management platform that consists of three main components: Personnel Information (Employee File) Management, Leave Management, and Time & Attendance Management, providing you with a user friendly, and robust solution.
</p>