<?php
$type = $variables['type'];
?>

<p>Dear @[name],</p>
<?php if ($type == HRM_EMAIL_NOTIF_EMPLOYEE_ACCOUNT_NEW) { ?>
  <p>
    Please note that your account on @[responsehrm_link] has been created.
  </p>
  <p>
    You are advised to install @[responsehrm_app_link] from Google Play Store. Once installed on your android devise, please login using the below account credentials:
  </p>
  <p>
    Email: @[email]<br />
    Password: @[password]
  </p>
<?php }
elseif ($type == HRM_EMAIL_NOTIF_EMPLOYEE_ACCOUNT_RESET_PASSWORD) { ?>
  <p>
    A request to reset the password for your account has been made at @[responsehrm_link]. You may now login using the below account credentials:
  </p>
  <p>
    Email: @[email]<br />
    Password: @[password]
  </p>
<?php } ?>
<p>
  Best Regards,<br />
  @[organization_name]
</p>
<p>
  @[responsehrm_name] Administrator
</p>

<hr/>
<p class="footer-message">
  @[responsehrm_name] is a human resource management solution that is feature-rich, intuitive and provides an essential HR management platform that consists of three main components: Personnel Information (Employee File) Management, Leave Management, and Time & Attendance Management, providing you with a user friendly, and robust solution.
</p>